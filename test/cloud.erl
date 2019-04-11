-module(cloud).

-include_lib("eunit/include/eunit.hrl").


cloud_env() ->
  Credentials = [{access_key_id, "admin"},
                 {secret_key, "password"}],
  AwsOptions =  [{endpoint_override, "127.0.0.1:9000"}, {scheme, "http"}],
  EnvOptions = [{credentials, Credentials}, {aws_options, AwsOptions}],
  rocksdb:new_aws_env("test", "", "", "test", "", "", EnvOptions).


open_db() ->
  {ok, CloudEnv} = cloud_env(),
  DbOptions =  [{create_if_missing, true}, {env, CloudEnv}],
  rocksdb:open_cloud_db("cloud_db", DbOptions, "/tmp/test", 128 bsl 20).


close_db(Db) ->
  ok = rocksdb:flush(Db, []),
  rocksdb:close(Db).

destroy_db() ->
  rocksdb:destroy("cloud_db", []).


basic_test() ->
  os:cmd("rm -rf cloud_db"),
  try
    {ok, Db} = open_db(),

    ok = rocksdb:put(Db, <<"key">>, <<"value">>, []),
    {ok, <<"value">>} = rocksdb:get(Db, <<"key">>, []),

    ok =close_db(Db),

    {ok, Db1} = open_db(),
    {ok, <<"value">>} = rocksdb:get(Db1, <<"key">>, []),
    close_db(Db1)
  after
    destroy_db()
  end.


