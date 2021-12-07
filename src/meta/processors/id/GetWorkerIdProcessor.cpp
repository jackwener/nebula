/* Copyright (c) 2021 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "meta/processors/id/GetWorkerIdProcessor.h"

namespace nebula {
namespace meta {

void GetWorkerIdProcessor::process(const cpp2::GetWorkerIdReq& req) {
  const string& ipAddr = req.get_ip_address();
  auto result = doGet(ipAddr);
  if (nebula::ok(result)) {
    string workerIdStr = std::move(nebula::value(result));
    int32_t workerIdInt32 = std::stoi(workerIdStr);

    handleErrorCode(nebula::cpp2::ErrorCode::SUCCEEDED);
    resp_.set_workerid(std::move(workerIdInt32));
    return;
  }

  auto newResult = doGet(idKey);
  if (!nebula::ok(newResult)) {
    LOG(ERROR) << "Get idKey worker id failed";
    // TODO handleErrorCode(nebula::cpp2::ErrorCode::E_GET_WORKER_ID_FAILED);
    return;
  }
  folly::SharedMutex::WriteHolder wHolder(LockUtils::workIdLock());

  string workerIdStr = std::move(nebula::value(newResult));
  int32_t workerIdInt32 = std::stoi(workerIdStr);

  int32_t newWorkerId = workerIdInt32 + 1;
  doPut(std::vector<kvstore::KV>{{idKey, std::to_string(newWorkerId)}});

  handleErrorCode(nebula::cpp2::ErrorCode::SUCCEEDED);
  resp_.set_workerid(std::move(workerIdInt32));
}

}  // namespace meta
}  // namespace nebula
