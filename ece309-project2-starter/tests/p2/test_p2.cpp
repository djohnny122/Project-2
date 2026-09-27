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
    // Test 1: Empty Conversation
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
    //Test 2: Append and Access Messages
    {
        // Create a conversation
        Conversation convo2;
        // Append messages
        convo2.append(Message(Role::User, "Hello"));
        convo2.append(Message(Role::Assistant, "Hi!"));
        // Sanity checks for size, roles, and content of the messages
        assert(convo2.size() == 2);
        assert(convo2.at(0).role() == Role::User);
        assert(convo2.at(0).content() == "Hello");
        assert(convo2.at(1).role() == Role::Assistant);
        assert(convo2.at(1).content() == "Hi!");
    }
    //Test 3: 
    return 0;
}
