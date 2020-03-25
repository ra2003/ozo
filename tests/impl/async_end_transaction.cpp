#include "connection_mock.h"

#include <ozo/core/options.h>
#include <ozo/transaction.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace {

using namespace testing;
using namespace ozo::tests;

using ozo::error_code;
using ozo::time_traits;

struct async_end_transaction : Test {
    StrictMock<connection_gmock> connection {};
    StrictMock<executor_mock> callback_executor{};
    StrictMock<callback_gmock<connection_ptr<>>> callback {};
    StrictMock<executor_mock> strand {};
    io_context io;
    StrictMock<PGconn_mock> handle;
    connection_ptr<> conn = make_connection(connection, io, handle);
    decltype(ozo::make_options()) options = ozo::make_options();
    time_traits::duration timeout {42};
};

TEST_F(async_end_transaction, should_call_async_execute) {
    EXPECT_CALL(handle, PQstatus()).WillRepeatedly(Return(CONNECTION_OK));

    auto transaction = ozo::transaction(std::move(conn), options);

    const InSequence s;

    EXPECT_CALL(connection, async_execute()).WillOnce(Return());

    ozo::detail::async_end_transaction(std::move(transaction), empty_query {}, timeout, wrap(callback));
}

} // namespace
