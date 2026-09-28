// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

#include "core/conversation.h"
#include "core/message.h"
#include "core/sentinel_scanner.h"
#include "harness/harness.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"

#include <cassert>
#include <stdexcept>
#include <fstream>

int main() {
    // TODO: write your tests here.
    // Test 1: Empty Conversation Bounds
    {
        // Create an empty Conversation and check its size and begin/end pointers
        Conversation convo1;
        assert(convo1.size() == 0);
        assert(convo1.begin() == convo1.end());
        // Initialize a boolean variable to track if an exception is thrown
        bool threw = false;
        // Check that accessing an out-of-bounds index throws an exception
        try {
            convo1.at(0);
        }
        // Catch the out_of_range exception and set threw to true
        catch (const std::out_of_range&) {
            threw = true;
        }
        assert(threw);
    }
    //Test 2: System Message Ordering
    {
        //Create a Conversation
        Conversation convo2;
        //Append messages
        convo2.append(Message(Role::System, "System"));
        convo2.append(Message(Role::User, "User"));
        convo2.append(Message(Role::Assistant, "Assistant"));
        convo2.append(Message(Role::User, "User2"));
        //Check that the first message is a System message with the correct content
        assert(convo2.at(0).role() == Role::System);
        assert(convo2.at(0).content() == "System");
        //Check the other messages
        assert(convo2.at(1).role() == Role::User);
        assert(convo2.at(1).content() == "User");
        assert(convo2.at(2).role() == Role::Assistant);
        assert(convo2.at(2).content() == "Assistant");
        assert(convo2.at(3).role() == Role::User);
        assert(convo2.at(3).content() == "User2");
    }
    //Test 3: Rule of Five Copy
    {
        //Create a Conversation
        Conversation convo3;
        //Append messages
        convo3.append(Message(Role::User, "Hello"));
        convo3.append(Message(Role::Assistant, "Hi!"));
        //Create a copy of the Conversation
        Conversation copy(convo3);
        //Check that the copy has the same size and content as the original
        assert(copy.size() == convo3.size());
        assert(copy.at(0).role() == convo3.at(0).role());
        assert(copy.at(0).content() == convo3.at(0).content());
        assert(copy.at(1).role() == convo3.at(1).role());
        assert(copy.at(1).content() == convo3.at(1).content());
    }
    //Test 4: Rule of Five Move
    {
        //Create a Conversation
        Conversation convo4;
        //Append messages
        convo4.append(Message(Role::User, "Hello"));
        convo4.append(Message(Role::Assistant, "Hi!"));
        //Save original pointer
        const Message* original_ptr = convo4.begin();
        //Move the Conversation
        Conversation moved(std::move(convo4));
        //Check that the moved Conversation has the same size and content as the original
        assert(moved.size() == 2);
        assert(moved.at(0).role() == Role::User);
        assert(moved.at(0).content() == "Hello");
        assert(moved.at(1).role() == Role::Assistant);
        assert(moved.at(1).content() == "Hi!");
        //Check that the original Conversation is now empty
        assert(convo4.size() == 0);
        assert(convo4.begin() == convo4.end());
    }    
    //Test 5: Growth Behavior
    {
        //Create a Conversation
        Conversation convo5;

        const Message* orig_data = convo5.begin();
        //Append first message
        convo5.append(Message(Role::User, "User1"));
        //Capacity should have grown, so the data pointer should be different
        assert(convo5.begin() != orig_data); // Check that the data pointer has changed after appending
        orig_data = convo5.begin();

        //Append second message
        convo5.append(Message(Role::User, "User2"));
        //Capacity was 1, so it should have grown, and the data pointer should be different
        assert(convo5.begin() != orig_data);
        orig_data = convo5.begin();

        //Append third message
        convo5.append(Message(Role::User, "User3"));
        //Capacity was 2, so it should have grown, and the data pointer should be different
        assert(convo5.begin() != orig_data);
        orig_data = convo5.begin();

        //Append fourth message
        convo5.append(Message(Role::User, "User4"));
        //Capacity was 4, so it should not have grown, and the data pointer should be the same
        assert(convo5.begin() == orig_data);
        assert(convo5.size() == 4);

        //Check the content of the messages
        assert(convo5.at(0).content() == "User1");
        assert(convo5.at(1).content() == "User2");
        assert(convo5.at(2).content() == "User3");
        assert(convo5.at(3).content() == "User4");
    }
    //Test 6: Scanner Clean Text
    {
        // Create a SentinelScanner with a sentinel string
        SentinelScanner scanner("<|end_conversation|>");

        auto check1 = scanner.feed("Test string.");
        auto chunk_check = scanner.flush();

        //"Test string." does not contain the sentinel, so sentinel_found should be false
        assert(!check1.sentinel_found);
        assert(!chunk_check.sentinel_found);

        // The safe_text should be the same as the input text
        assert(check1.safe_text + chunk_check.safe_text == "Test string.");
    }
    //Test 7: Scanner Split Sentinel 
    {
    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye." + sentinel;

    for (std::size_t split = 0; split <= text.size(); ++split) {
        SentinelScanner scanner(sentinel);

        auto out1 = scanner.feed(text.substr(0, split));
        auto out2 = scanner.feed(text.substr(split));

        assert(out1.sentinel_found || out2.sentinel_found);
        assert(out1.safe_text + out2.safe_text == "Goodbye.");
    }
    }
    //Test 8: Scanner False Alarms
    {   
        // Create a SentinelScanner with a sentinel string
        SentinelScanner scanner("<|end_conversation|>");
        auto out1 = scanner.feed("<|end_world|>");
        auto out2 = scanner.flush();

        assert(!out1.sentinel_found);
        assert(!out2.sentinel_found);
        assert(out1.safe_text + out2.safe_text == "<|end_world|>");
    }
    //Test 9: Scanner Bounded Memory
    {     
        // Create a SentinelScanner with a sentinel string
        SentinelScanner scanner("<|end_conversation|>");

        for (std::size_t i = 0; i < (4*1024*1024); ++i) {
            char c ='x';
            auto out = scanner.feed(std::string_view(&c, 1));
            //The sentinel should not be found in this large input, so sentinel_found should be false
            assert(!out.sentinel_found);
        }
        auto out2 = scanner.flush();
        //The sentinel should not be found in the flush, so sentinel_found should be false
        assert(!out2.sentinel_found);
    }
    //Test 10: Harness Turn Limit
{   
    //Needed classes for the Harness test
    class MyInput : public InputSource {
    public:
        std::string read_line() override {
            return "hello";
        }

        bool is_eof() const override {
            return false;
        }
    };

    class MyOutput : public OutputSink {
    public:
        void write(std::string_view text) override {
        }
    };
    //Configure the Harness with a turn limit of 2
    HarnessConfig config;
    config.max_turns = 2;

    //Create a ScriptedModelClient
    auto model = std::make_unique<ScriptedModelClient>("scripts/greeting.script");

    Harness harness(std::move(model), config);
    //Run the Harness with the MyInput and MyOutput classes
    MyInput input;
    MyOutput output;
    StopReason result = harness.run(input, output);
    //The result should indicate that the turn limit was reached
    assert(result.kind == StopReason::Kind::TurnLimit);
}
    //Test 11: Harness Sentinel Halt
{
    //Needed classes for the Harness test
    class MyInput : public InputSource {
    public:
        std::string read_line() override {
            return "hello";
        }

        bool is_eof() const override {
            return false;
        }
    };

    class MyOutput : public OutputSink {
    public:
        void write(std::string_view text) override {
        }
    };
    //Configure the Harness with a turn limit of 10
    HarnessConfig config;
    config.max_turns = 10;
    //Create a ScriptedModelClient
    auto model = std::make_unique<ScriptedModelClient>("scripts/greeting.script");

    Harness harness(std::move(model), config);
    //Run the Harness with the MyInput and MyOutput classes
    MyInput input;
    MyOutput output;
    StopReason result = harness.run(input, output);
    //The result should indicate that the sentinel was found
    assert(result.kind == StopReason::Kind::Sentinel);
    }
    //Test 12: Transcript Round-Trip
    {
        // Create a transcript file with a user message and an assistant reply
        std::ofstream file("test_transcript.txt");
        file << "role: user\n";
        file << "Hello\n";
        file << "---\n";
        file << "role: assistant\n";
        file << "Hi!\n";

        file.close();
        // Create a ReplayModelClient with the transcript file
        ReplayModelClient replay("test_transcript.txt");
        // Create a Conversation with a user message
        Conversation conv;
        conv.append(Message(Role::User, "Hello"));
        // Generate a message from the ReplayModelClient
        Message msg = replay.generate(conv);
        // Check that the generated message is an assistant reply with the correct content
        assert(msg.role() == Role::Assistant);
        assert(msg.content() == "Hi!");
    }
    return 0;
}
