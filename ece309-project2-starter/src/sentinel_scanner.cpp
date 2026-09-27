#include "core/sentinel_scanner.h"

// SentinelScanner constructor initializes the sentinel string and pending string
SentinelScanner::SentinelScanner(std::string sentinel)
    : sentinel_(sentinel), pending_("") {
}

// feed function processes the next chunk of text and checks for the sentinel
SentinelScanner::Out SentinelScanner::feed(std::string_view chunk){
    std::string combined = pending_;
    combined += chunk;

    // Check if the sentinel is found in the combined string
    std::size_t pos = combined.find(sentinel_);
    // If the sentinel is found, return the safe text and indicate that the sentinel was found
    if (pos != std::string::npos) {
        // Clear pending_ since the sentinel has been found
        pending_.clear();

        // Return the safe text (up to the position of the sentinel) and indicate that the sentinel was found
        return{combined.substr(0, pos), true};
    }

    // If the sentinel is not found, determine how much of the combined string to keep in pending_
    std::size_t keep = 0;
    // If the sentinel is longer than 1 character, we need to keep at most sentinel_.size() - 1 trailing characters that could still become the start of the sentinel
    if (sentinel_.size() > 1) {
        // Keep at most sentinel_.size() - 1 trailing characters that could still become the start of the sentinel
        keep = sentinel_.size() - 1;
    }
    // If the combined string is smaller than or equal to the number of characters to keep, store it in pending_ and return an empty safe text
    if (combined.size() <= keep){
        pending_ = combined;
        return{ "", false };
    }

    // If the combined string is larger than the number of characters to keep, return the safe text and update pending_
    std::size_t safe_size = combined.size() - keep;
    std::string safe_text = combined.substr(0, safe_size);
    pending_ = combined.substr(safe_size);
    return {safe_text, false};
}

// flush function releases any text still being held back and checks for the sentinel
SentinelScanner::Out SentinelScanner::flush(){
    std::string safe_text = pending_;
    pending_.clear();
    return {safe_text, false};
}