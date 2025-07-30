REBAR ?= $(shell which rebar3 2>/dev/null)

TEST_DIR=$(CURDIR)/test


TEST_MODULES="db,db_range,iterators,batch,snapshot,column_family,batch,cache,blob_db,checkpoint,db_backup,cleanup,in_mem,merge,rate_limiter,sst_file_manager,transaction,transaction_log,ttl,write_buffer_manager,statistics,fifo_compaction"

TEST_ALL_MODULES="${TEST_MODULES},compression"

.PHONY: all
all: compile test

.PHONY: compile
compile:
	@${REBAR} compile

.PHONY: test
test: compile
	@${REBAR} eunit --module=${TEST_MODULES}

.PHONY: test_all
test_all: compile
	@${REBAR} eunit --module=${TEST_ALL_MODULES}

.PHONY: clean
clean:
	@${REBAR} clean

.PHONY: distclean
distclean: clean
	@git clean -fdx
