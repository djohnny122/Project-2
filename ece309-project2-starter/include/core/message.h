#include <string>

enum class Role { System, User, Assistant };

class Message {
public:
    // Default-constructs an empty System message with empty content.
    // Needed so Conversation can allocate raw array slots before
    // append() fills them in.
    Message()
        // Default values for role and content
        : role_(Role::System), content_("") {
    }

    Message(Role role, std::string content)
        // Constructs a message with the given role and content.
        : role_(role), content_(content) {
    }

    // Who sent this message.
    Role               role()    const noexcept{
        // Return the role of the message
        return role_;
    }  
    
    // The message text.
    const std::string& content() const noexcept{
        // Return a const reference to the content string
        return content_;
    }  

private:
    Role        role_;
    std::string content_;
};