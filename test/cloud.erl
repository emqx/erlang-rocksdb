-module(cloud).

-include_lib("eunit/include/eunit.hrl").


access_key() ->
  os:getenv("MINIO_ACCESS_KEY", "admin").

secret_key()->
  os:getenv("MINIO_SECRET_KEY", "password").

endpoint() ->
  os:getenv("ENDPOINT", "127.0.0.1:9000").


cloud_env() ->
  Credentials = [{access_key_id, access_key()},
                 {secret_key, secret_key()}],
  AwsOptions =  [{endpoint_override, endpoint()}, {scheme, "http"}],
  EnvOptions = [{credentials, Credentials}, {aws_options, AwsOptions}],
  {ok, CloudEnv} = rocksdb:new_cloud_env("test", "", "", "test", "", "", EnvOptions),
  ok = rocksdb:cloud_env_empty_bucket(CloudEnv, "", "/"),
  rocksdb:new_cloud_env("test", "", "", "test", "", "", EnvOptions).


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


