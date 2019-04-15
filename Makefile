REBAR ?= ./deps/rebar3
MINIO_PID=/tmp/erocksdb_minio.pid

all: compile test

compile:
	@${REBAR} compile

test: compile
	@rm -rf /tmp/erocksdb_data
	@rm -rf $(MINIO_PID)
	@mkdir -p /tmp/erocksdb_data
	@MINIO_ACCESS_KEY="admin" \
		MINIO_SECRET_KEY="password" \
		minio server /tmp/erocksdb_data & \
		echo "$$!" > $(MINIO_PID)
	@${REBAR} eunit
	@kill `cat $(MINIO_PID)`

clean:
	@${REBAR} clean
