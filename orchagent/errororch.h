/*
 * Copyright 2019 Broadcom.  The term Broadcom refers to Broadcom Inc. and/or
 * its subsidiaries.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SWSS_ERRORORCH_H
#define SWSS_ERRORORCH_H

#include "orch.h"
#include "notificationproducer.h"
#include "notificationconsumer.h"
#include <map>

using namespace std;
using namespace swss;

typedef enum {
    ERRORORCH_FLAGS_NOTIF_SEND = 0x1,
    ERRORORCH_FLAGS_LAST
} errororch_flags_t;
class ErrorOrch: public Orch
{
    public:
        ErrorOrch(DBConnector *asicDb, DBConnector *errorDb, vector<string> &tableNames);
        string getErrorListenerChannelName(string &appDbTableName);
        string getErrorTableName(string &appDbTableName);
        string getAppTableName(string &errDbTableName);
        bool mappingHandlerRegister(string tableName, Orch* orch);
        bool mappingHandlerDeRegister(string tableName);
        bool createTableObject(string &errTableName);
        bool deleteTableObject(string &errTableName);
        bool applNotificationEnabled(_In_ sai_object_type_t object_type);
        void sendNotification(_In_ string& tableName, _In_ string& op,
                _In_ string& data, _In_ vector<FieldValueTuple> &entry);
        void sendNotification(_In_ string& tableName,
                _In_ string& op, _In_ string& data);
        void addErrorEntry(sai_object_type_t object_type,
                vector<FieldValueTuple> &appValues, uint32_t flags);

    private:
        shared_ptr<DBConnector> m_errorDb;
        NotificationConsumer* m_errorNotificationConsumer;
        NotificationConsumer* m_errorFlushNotificationConsumer;
        /* Table ID to Orchestration agent object map */
        map<string, Orch*>  m_TableOrchMap;
        map<string, shared_ptr<Table>> m_TableNameObjMap;
        unordered_map<string, shared_ptr<NotificationProducer>> m_TableChannel;

        void doTask(Consumer &consumer);
        void doTask(NotificationConsumer& consumer);
        void extractEntry(vector<FieldValueTuple> &values,
                const string &field, string &value);
        int flushErrorDb(const string &op, const string &tableName);
        void updateErrorDb(string &tableName, const string &key,
                vector<FieldValueTuple> &values);

};

#endif /* SWSS_ERRORORCH_H */
