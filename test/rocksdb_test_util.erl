%% Copyright (c) 2009,2010 Dave Smith (dizzyd@dizzyd.com)

-module(rocksdb_test_util).


-export([rm_rf/1,
         sh/2]).

-define(FAIL, abort()).
-define(ABORT(Str, Args), abort(Str, Args)).

-define(DEBUG(Str, Args), io:format(Str++"~n", Args)).
-define(ERROR(Str, Args), io:format(standard_error, Str++"~n", Args)).


-define(FMT(Str, Args), lists:flatten(io_lib:format(Str, Args))).


%% @doc Remove files and directories.
%% Target is a single filename, directoryname or wildcard expression.
-spec rm_rf(string()) -> 'ok'.
rm_rf(Target) ->
  case os:type() of
    {unix, _} ->
      EscTarget = escape_chars(Target),
      {ok, []} = sh(?FMT("rm -rf ~ts", [EscTarget]),
                    [{use_stdout, false}, abort_on_error]),
      ok;
    {win32, _} ->
      Filelist = filelib:wildcard(Target),
      Dirs = [F || F <- Filelist, filelib:is_dir(F)],
      Files = Filelist -- Dirs,
      ok = delete_each(Files),
      ok = delete_each_dir_win32(Dirs),
      ok
  end.

delete_each([]) ->
  ok;
delete_each([File | Rest]) ->
  case file:delete(File) of
    ok ->
      delete_each(Rest);
    {error, enoent} ->
      delete_each(Rest);
    {error, Reason} ->
      ?ERROR("Failed to delete file ~ts: ~p\n", [File, Reason]),
      ?FAIL
  end.

delete_each_dir_win32([]) -> ok;
delete_each_dir_win32([Dir | Rest]) ->
  {ok, []} = sh(?FMT("rd /q /s \"~ts\"",
                     [escape_double_quotes(filename:nativename(Dir))]),
                [{use_stdout, false}, return_on_error]),
  delete_each_dir_win32(Rest).



%%
%% Options = [Option] -- defaults to [use_stdout, abort_on_error]
%% Option = ErrorOption | OutputOption | {cd, string()} | {env, Env}
%% ErrorOption = return_on_error | abort_on_error | {abort_on_error, string()}
%% OutputOption = use_stdout | {use_stdout, bool()}
%% Env = [{string(), Val}]
%% Val = string() | false
%%
sh(Command0, Options0) ->
  %%?DEBUG("sh info:\n\tcwd: ~p\n\tcmd: ~ts\n", [rebar_dir:get_cwd(), Command0]),
  %%?DEBUG("\topts: ~p\n", [Options0]),

  DefaultOptions = [{use_stdout, false}, debug_and_abort_on_error],
  Options = [expand_sh_flag(V)
             || V <- proplists:compact(Options0 ++ DefaultOptions)],

  ErrorHandler = proplists:get_value(error_handler, Options),
  OutputHandler = proplists:get_value(output_handler, Options),

  Command = lists:flatten(patch_on_windows(Command0, proplists:get_value(env, Options0, []))),
  PortSettings = proplists:get_all_values(port_settings, Options) ++
  [exit_status, {line, 16384}, use_stdio, stderr_to_stdout, hide, eof],
  %%?DEBUG("Port Cmd: ~ts\nPort Opts: ~p\n", [Command, PortSettings]),
  Port = open_port({spawn, Command}, PortSettings),

  try
    case sh_loop(Port, OutputHandler, []) of
      {ok, _Output} = Ok ->
        Ok;
      {error, {_Rc, _Output}=Err} ->
        ErrorHandler(Command, Err)
    end
  after
    port_close(Port)
  end.


%% @doc Given env. variable `FOO' we want to expand all references to
%% it in `InStr'. References can have two forms: `$FOO' and `${FOO}'
%% The end of form `$FOO' is delimited with whitespace or EOL
-spec expand_env_variable(string(), string(), term()) -> string().
expand_env_variable(InStr, VarName, RawVarValue) ->
  case rebar_string:chr(InStr, $$) of
    0 ->
      %% No variables to expand
      InStr;
    _ ->
      ReOpts = [global, unicode, {return, list}],
      VarValue = re:replace(RawVarValue, "\\\\", "\\\\\\\\", ReOpts),
      %% Use a regex to match/replace:
      %% Given variable "FOO": match $FOO\s | $FOOeol | ${FOO}
      RegEx = io_lib:format("\\\$(~ts(\\W|$)|{~ts})", [VarName, VarName]),
      re:replace(InStr, RegEx, [VarValue, "\\2"], ReOpts)
  end.

%% We do the shell variable substitution ourselves on Windows and hope that the
%% command doesn't use any other shell magic.
patch_on_windows(Cmd, Env) ->
  case os:type() of
    {win32,nt} ->
      Cmd1 = "cmd /q /c "
      ++ lists:foldl(fun({Key, Value}, Acc) ->
                         expand_env_variable(Acc, Key, Value)
                     end, Cmd, Env),
      %% Remove left-over vars
      re:replace(Cmd1, "\\\$\\w+|\\\${\\w+}", "",
                 [global, {return, list}, unicode]);
    _ ->
      Cmd
  end.

expand_sh_flag(return_on_error) ->
  {error_handler,
   fun(_Command, Err) ->
       {error, Err}
   end};
expand_sh_flag(abort_on_error) ->
  {error_handler,
   fun log_and_abort/2};
expand_sh_flag({abort_on_error, Message}) ->
  {error_handler,
   log_msg_and_abort(Message)};
expand_sh_flag({debug_abort_on_error, Message}) ->
  {error_handler,
   debug_log_msg_and_abort(Message)};
expand_sh_flag(debug_and_abort_on_error) ->
  {error_handler,
   fun debug_and_abort/2};
expand_sh_flag(use_stdout) ->
  {output_handler,
   fun(Line, Acc) ->
       %% Line already has a newline so don't use ?CONSOLE which adds one
       io:format("~ts", [Line]),
       [Line | Acc]
   end};
expand_sh_flag({use_stdout, false}) ->
  {output_handler,
   fun(Line, Acc) ->
       [Line | Acc]
   end};
expand_sh_flag({cd, _CdArg} = Cd) ->
  {port_settings, Cd};
expand_sh_flag({env, _EnvArg} = Env) ->
  {port_settings, Env}.

-type err_handler() :: fun((string(), {integer(), string()}) -> no_return()).
-spec log_msg_and_abort(string()) -> err_handler().
log_msg_and_abort(Message) ->
  fun(_Command, {_Rc, _Output}) ->
      ?ABORT(Message, [])
  end.

-spec debug_log_msg_and_abort(string()) -> err_handler().
debug_log_msg_and_abort(Message) ->
  fun(Command, {Rc, Output}) ->
      ?DEBUG("sh(~ts)~n"
             "failed with return code ~w and the following output:~n"
             "~ts", [Command, Rc, Output]),
      ?ABORT(Message, [])
  end.

-spec log_and_abort(string(), {integer(), string()}) -> no_return().
log_and_abort(Command, {Rc, Output}) ->
  ?ABORT("sh(~ts)~n"
         "failed with return code ~w and the following output:~n"
         "~ts", [Command, Rc, Output]).

-spec debug_and_abort(string(), {integer(), string()}) -> no_return().
debug_and_abort(Command, {Rc, Output}) ->
  ?DEBUG("sh(~ts)~n"
         "failed with return code ~w and the following output:~n"
         "~ts", [Command, Rc, Output]),
  throw(rebar_abort).

sh_loop(Port, Fun, Acc) ->
  receive
    {Port, {data, {eol, Line}}} ->
      sh_loop(Port, Fun, Fun(Line ++ "\n", Acc));
    {Port, {data, {noeol, Line}}} ->
      sh_loop(Port, Fun, Fun(Line, Acc));
    {Port, eof} ->
      Data = lists:flatten(lists:reverse(Acc)),
      receive
        {Port, {exit_status, 0}} ->
          {ok, Data};
        {Port, {exit_status, Rc}} ->
          {error, {Rc, Data}}
      end
  end.

%% escape\ as\ a\ shell\?
escape_chars(Str) when is_atom(Str) ->
  escape_chars(atom_to_list(Str));
escape_chars(Str) ->
  re:replace(Str, "([ ()?`!$&;\"\'])", "\\\\&",
             [global, {return, list}, unicode]).

%% "escape inside these"
escape_double_quotes(Str) ->
  re:replace(Str, "([\"\\\\`!$&*;])", "\\\\&",
             [global, {return, list}, unicode]).

-spec abort() -> no_return().
abort() ->
  throw(rebar_abort).

-spec abort(string(), [term()]) -> no_return().
abort(String, Args) ->
  ?ERROR(String, Args),
  abort().

