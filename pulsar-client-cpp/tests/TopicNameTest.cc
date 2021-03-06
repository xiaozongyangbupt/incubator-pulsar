/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include <gtest/gtest.h>
#include <lib/TopicName.h>

using namespace pulsar;

TEST(TopicNameTest, testLookup) {
    boost::shared_ptr<TopicName> topicName = TopicName::get("persistent://pulsar/bf1/TESTNS.0/curveballapps");
    std::string lookup_name = topicName->getLookupName();
    ASSERT_EQ(lookup_name, "persistent/pulsar/bf1/TESTNS.0/curveballapps");
}

TEST(TopicNameTest, testTopicName) {
    // Compare getters and setters
    boost::shared_ptr<TopicName> topicName = TopicName::get("persistent://property/cluster/namespace/topic");
    ASSERT_EQ("property", topicName->getProperty());
    ASSERT_EQ("cluster", topicName->getCluster());
    ASSERT_EQ("namespace", topicName->getNamespacePortion());
    ASSERT_EQ("persistent", topicName->getDomain());
    ASSERT_EQ(TopicName::getEncodedName("topic"), topicName->getLocalName());

    // Compare == operator
    boost::shared_ptr<TopicName> topicName1 = TopicName::get("persistent://p/c/n/d");
    boost::shared_ptr<TopicName> topicName2 = TopicName::get("persistent://p/c/n/d");
    ASSERT_TRUE(*topicName1 == *topicName2);
}

TEST(TopicNameTest, testTopicNameWithSlashes) {
    // Compare getters and setters
    boost::shared_ptr<TopicName> topicName =
        TopicName::get("persistent://property/cluster/namespace/topic/name/with/slash");
    ASSERT_EQ("property", topicName->getProperty());
    ASSERT_EQ("cluster", topicName->getCluster());
    ASSERT_EQ("namespace", topicName->getNamespacePortion());
    ASSERT_EQ("persistent", topicName->getDomain());
    ASSERT_EQ("topic/name/with/slash", topicName->getLocalName());

    topicName = TopicName::get("persistent://property/cluster/namespace/topic/ends/with/slash/");
    ASSERT_TRUE(topicName != NULL);
    ASSERT_EQ(TopicName::getEncodedName("topic/ends/with/slash/"), topicName->getEncodedLocalName());

    topicName = TopicName::get("persistent://property/cluster/namespace/`~!@#$%^&*()-_+=[]{}|\\;:'\"<>,./?");
    ASSERT_TRUE(topicName != NULL);
    ASSERT_EQ(TopicName::getEncodedName("`~!@#$%^&*()-_+=[]{}|\\;:'\"<>,./?"),
              topicName->getEncodedLocalName());

    topicName = TopicName::get("persistent://property/cluster/namespace/topic@%*)(&!%$#@#$><?");
    ASSERT_TRUE(topicName != NULL);
    ASSERT_EQ(TopicName::getEncodedName("topic@%*)(&!%$#@#$><?"), topicName->getEncodedLocalName());

    topicName = TopicName::get("persistent://property/cluster/namespace/topic//with//double//slash//");
    ASSERT_TRUE(topicName != NULL);
    ASSERT_EQ(TopicName::getEncodedName("topic//with//double//slash//"), topicName->getEncodedLocalName());

    topicName = TopicName::get("persistent://property/cluster/namespace//topic/starts/with/slash/");
    ASSERT_TRUE(topicName != NULL);
    ASSERT_EQ(TopicName::getEncodedName("/topic/starts/with/slash/"), topicName->getEncodedLocalName());
}
TEST(TopicNameTest, testEmptyClusterName) {
    // Compare getters and setters
    boost::shared_ptr<TopicName> topicName = TopicName::get("persistent://property//namespace/topic");

    ASSERT_FALSE(topicName);
}

TEST(TopicNameTest, testExtraSlashes) {
    boost::shared_ptr<TopicName> topicName = TopicName::get("persistent://property/cluster//namespace/topic");
    ASSERT_FALSE(topicName);
    topicName = TopicName::get("persistent://property//cluster//namespace//topic");
    ASSERT_FALSE(topicName);
}

TEST(TopicNameTest, testIllegalCharacters) {
    boost::shared_ptr<TopicName> topicName =
        TopicName::get("persistent://prop!!!erty/cluster&)&Name/name%%%space/topic");
    ASSERT_FALSE(topicName);
}

TEST(TopicNameTest, testIllegalUrl) {
    boost::shared_ptr<TopicName> topicName = TopicName::get("persistent:::/property/cluster/namespace/topic");
    ASSERT_FALSE(topicName);
}

TEST(TopicNameTest, testEmptyString) {
    boost::shared_ptr<TopicName> topicName = TopicName::get("");
    ASSERT_FALSE(topicName);
}

TEST(TopicNameTest, testExtraArguments) {
    boost::shared_ptr<TopicName> topicName =
        TopicName::get("persistent:::/property/cluster/namespace/topic/some/extra/args");
    ASSERT_FALSE(topicName);
}
