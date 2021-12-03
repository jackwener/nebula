/* Copyright (c) 2021 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef META_GETWORKERIDPROCESSOR_H_
#define META_GETWORKERIDPROCESSOR_H_

#include "meta/processors/BaseProcessor.h"

namespace nebula {
namespace meta {

class GetWorkerIdProcessor : public BaseProcessor<cpp2::GetWorkerIdResp> {
 public:
  static GetWorkerIdProcessor* instance(kvstore::KVStore* kvstore) {
    return new GetWorkerIdProcessor(kvstore);
  }

  void process(const cpp2::GetWorkerIdReq& req);

 private:
  mutable std::mutex lock_;

  explicit GetWorkerIdProcessor(kvstore::KVStore* kvstore)
      : BaseProcessor<cpp2::GetWorkerIdResp>(kvstore) {
    std::vector<kvstore::KV> data = {{idKey, "0"}};
    doPut(data);
  }

  inline static const string idKey = "snowflake_worker_id";
};

}  // namespace meta
}  // namespace nebula

#endif  // META_GETWORKERIDPROCESSOR_H_
