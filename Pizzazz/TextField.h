#pragma once
#include "pizzazz.h"
#include <string>
#include <vector>

namespace pizzazz {

    /* Gets a line of input while showing autocomplete suggestions.
       Press Tab to accept a suggestion. Optional parameters:
       * a default message to show
       * whether to require choosing a suggestion
       * whether to require case sensitivity */
    std::string getline_autocompleted(
        const std::vector<std::string>& suggestions,
        std::string = "",
        bool = true,
        bool = false);

    class TextField {
    public:
        TextField(const std::vector<std::string>& suggestions, std::string default_message, bool must_use_suggestion, bool case_sensitive);
        std::string getline_autocompleted();
    private:
        std::string key = "";
        std::string input = "";
        size_t input_index = 0;
        std::string latest_suggestion = "";
        Coord start = get_cursor_coords();
        Coord current = start;
        Coord input_end = current;
        Coord suggestion_end = current;
        std::vector<std::string> suggestions;
        std::string default_message;
        bool must_use_suggestion;
        bool case_sensitive;

        void find_and_print_suggestion();
        std::string find_suggestion();
        bool is_suggestion(std::string& str);
        void clear_suggestion();
        void red_flash_text();
        void print_suggestion(std::string suggestion);
        int find_previous_space();
        int find_next_space();
        std::string kp_enter();
        std::string kp_tab();
        void kp_char();
        void kp_backspace();
        void kp_ctrl_backspace();
        void kp_delete();
        void kp_ctrl_delete();
        void kp_left_arrow();
        void kp_right_arrow();
        void kp_up_arrow();
        void kp_down_arrow();
        void kp_home();
        void kp_end();
        void kp_ctrl_left_arrow();
        void kp_ctrl_right_arrow();
    };
}
