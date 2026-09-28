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
    //Test 5: Growth Behavior
    //Test 6: Scanner Clean Text
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
    //Test 9: Scanner Bounded Memory
    //Test 10: Harness Turn Limit
    //Test 11: Harness Sentinel Halt
    //Test 12: Transcript Round-Trip
    return 0;
}
