#pragma once
#include "common.h"
#include <string>
#include <vector>
#include <optional>  // C++17+

namespace pizzazz {

    /* Gets a line of input while showing autocomplete suggestions (not autocorrect).
       Press Tab to accept a suggestion. Optional parameters:
       * a default message to show
       * whether to require choosing a suggestion
       * whether to require case sensitivity
       * whether to show suggestions */
    std::string getline_ac(
        const std::vector<std::string>& suggestions,
        std::string = "",
        bool = true,
        bool = false,
        bool = true);

    class TextField {
    public:
        TextField(const std::vector<std::string>& suggestions, std::string default_message, bool must_use_suggestion, bool case_sensitive, bool show_suggestions);
        std::string getline_ac();
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
        bool show_suggestions;

        void find_and_print_suggestion();
        std::string find_suggestion();
        bool is_suggestion(std::string& str);
        void clear_suggestion();
        void red_flash_text();
        void print_suggestion(std::string suggestion);
        int find_previous_space();
        int find_next_space();
        std::optional<std::string> kp_enter();
        std::optional<std::string> kp_tab();
        void key_char();
        void key_backspace();
        void key_ctrl_backspace();
        void key_delete();
        void key_ctrl_delete();
        void key_left_arrow();
        void key_right_arrow();
        void key_up_arrow();
        void key_down_arrow();
        void key_home();
        void key_end();
        void key_ctrl_left_arrow();
        void key_ctrl_right_arrow();
    };
}
