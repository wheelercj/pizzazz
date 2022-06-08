#include "TextField.h"

namespace pizzazz {

    std::string getline_ac(
            const std::vector<std::string>& suggestions,
            std::string default_message,
            opt options) {
        bool must_use_suggestion = !(int(options) & int(opt::no_validation));
        bool case_sensitive = int(options) & int(opt::case_sensitive);
        bool show_suggestions = !(int(options) & int(opt::hide_suggestions));
        TextField tf(suggestions, default_message, must_use_suggestion, case_sensitive, show_suggestions);
        return tf.getline_ac();
    }

    TextField::TextField(
            const std::vector<std::string>& suggestions,
            std::string default_message,
            bool must_use_suggestion,
            bool case_sensitive,
            bool show_suggestions) {
        this->suggestions = suggestions;
        this->default_message = default_message;
        this->must_use_suggestion = must_use_suggestion;
        this->case_sensitive = case_sensitive;
        this->show_suggestions = show_suggestions;
        if (default_message.size()) {
            find_and_print_suggestion();
            this->latest_suggestion = default_message;
        }
    }

    std::string TextField::getline_ac() {
        std::optional<std::string> result;
        while (true) {
            this->key = get_key();
            if (this->key == "enter") {
                result = key_enter();
                if (result && (result->size() || !must_use_suggestion || is_suggestion(*result)))
                    return *result;
            }
            else if (this->key == "tab") {
                result = key_tab();
                if (result && (result->size() || is_suggestion(*result)))
                    return *result;
            }
            else if (this->key.size() == 1)
                key_char();
            else if (this->key == "backspace"
                    && this->current.x > this->start.x
                    && this->input.size())
                key_backspace();
            else if (this->key == "ctrl+backspace"
                    && this->current.x > this->start.x
                    && this->input.size())
                key_ctrl_backspace();
            else if (this->key == "delete"
                    && this->current.x < this->input_end.x)
                key_delete();
            else if (this->key == "ctrl+delete"
                    && this->current.x < this->input_end.x)
                key_ctrl_delete();
            else if (this->key == "left arrow"
                    && this->current.x > this->start.x)
                key_left_arrow();
            else if (this->key == "right arrow"
                    && this->current.x < this->input_end.x)
                key_right_arrow();
            else if (this->key == "up arrow")
                key_up_arrow();
            else if (this->key == "down arrow")
                key_down_arrow();
            else if (this->key == "home")
                key_home();
            else if (this->key == "end")
                key_end();
            else if (this->key == "ctrl+left arrow"
                    && this->input_index > 0)
                key_ctrl_left_arrow();
            else if (this->key == "ctrl+right arrow"
                    && this->input_index < this->input.size())
                key_ctrl_right_arrow();
        }
    }

    void TextField::find_and_print_suggestion() {
        clear_suggestion();
        if (this->input.empty()) {
            if (show_suggestions && this->default_message.size())
                print_suggestion(this->default_message);
            return;
        }
        this->latest_suggestion = find_suggestion();
        if (show_suggestions)
            print_suggestion(this->latest_suggestion);
        set_cursor_coords(this->current);
    }

    std::string TextField::find_suggestion() {
        for (auto it = this->suggestions.begin(); it != this->suggestions.end(); it++) {
            std::string suggestion = *it;
            std::string lc_suggestion = suggestion;
            std::string lc_str = this->input;
            if (!this->case_sensitive) {
                lc_str = to_lower(lc_str);
                lc_suggestion = to_lower(lc_suggestion);
            }
            if (lc_suggestion.find(lc_str) == 0)
                return suggestion;
        }
        return "";
    }

    /* This function changes the capitalization of the parameter to match the
       suggestion if the function returns true. */
    bool TextField::is_suggestion(std::string& str) {
        for (auto it = this->suggestions.begin(); it != this->suggestions.end(); it++) {
            std::string suggestion = *it;
            std::string lc_suggestion = suggestion;
            std::string lc_str = str;
            if (!this->case_sensitive) {
                lc_str = to_lower(lc_str);
                lc_suggestion = to_lower(lc_suggestion);
            }
            if (lc_suggestion == lc_str) {
                str = suggestion;
                return true;
            }
        }
        return false;
    }

    void TextField::clear_suggestion() {
        set_cursor_coords(this->input_end);
        delete_chars(this->suggestion_end.x - this->input_end.x);
        this->suggestion_end = this->input_end;
        this->latest_suggestion = "";
    }

    void TextField::red_flash_text() {
        set_cursor_coords(this->start);
        print_styled(this->input, { Style::red });
        sleep_(300);
        set_cursor_coords(this->start);
        std::cout << this->input;
        set_cursor_coords(this->current);
    }

    void TextField::print_suggestion(std::string suggestion) {
        if (suggestion.size() <= this->input.size())
            return;
        std::string suffix = suggestion.substr(this->input.size());
        print_styled(suffix, { Style::bright_gray });
        this->suggestion_end = get_cursor_coords();
        set_cursor_coords(this->current);
    }

    int TextField::find_previous_space() {
        int i = int(this->input_index) - 1;
        if (i < 0)
            return int(this->input_index);
        while (i > 0 && this->input[i] != ' ')
            i--;
        return i;
    }

    int TextField::find_next_space() {
        int i = int(this->input_index) + 1;
        if (i == this->input.size())
            return int(this->input_index);
        while (i < this->input.size() && this->input[i] != ' ')
            i++;
        return i;
    }

    std::optional<std::string> TextField::key_enter() {
        if (!this->must_use_suggestion) {
            clear_suggestion();
            return this->input;
        }
        if (is_suggestion(this->input))
            return this->input;
        red_flash_text();
        return {};
    }

    std::optional<std::string> TextField::key_tab() {
        if (is_suggestion(this->input))
            return this->input;
        if (is_suggestion(this->latest_suggestion)) {
            set_cursor_coords(this->start);
            std::cout << this->latest_suggestion;
            return this->latest_suggestion;
        }
        return {};
    }

    void TextField::key_char() {
        if (this->input_index < this->input.size())
            this->input[this->input_index] = this->key[0];
        else {
            this->input.append(this->key);
            this->input_end.x += 1;
        }
        std::cout << this->key;
        this->current.x += 1;
        this->input_index += 1;
        find_and_print_suggestion();
    }

    void TextField::key_backspace() {
        this->input.erase(this->input_index - 1, 1);
        backspace_chars(1);
        this->current.x -= 1;
        this->input_end.x -= 1;
        this->input_index -= 1;
        find_and_print_suggestion();
    }

    void TextField::key_ctrl_backspace() {
        int s = find_previous_space();
        int i = int(this->input_index);
        int diff = i - s;
        backspace_chars(diff);
        this->input.erase(s, diff);
        this->current.x -= diff;
        this->input_end.x -= diff;
        this->input_index -= diff;
        find_and_print_suggestion();
    }

    void TextField::key_delete() {
        delete_chars(1);
        this->input_end.x -= 1;
        this->input.erase(this->input_index, 1);
        find_and_print_suggestion();
    }

    void TextField::key_ctrl_delete() {
        int s = find_next_space();
        int i = int(this->input_index);
        int diff = s - i;
        delete_chars(diff);
        this->input.erase(i, diff);
        this->input_end.x -= diff;
        find_and_print_suggestion();
    }

    void TextField::key_left_arrow() {
        this->current.x -= 1;
        this->input_index -= 1;
        set_cursor_coords(this->current);
    }

    void TextField::key_right_arrow() {
        this->current.x += 1;
        this->input_index += 1;
        set_cursor_coords(this->current);
    }

    void TextField::key_up_arrow() {
        // TODO
    }

    void TextField::key_down_arrow() {
        // TODO
    }

    void TextField::key_home() {
        this->current = this->start;
        this->input_index = 0;
        set_cursor_coords(this->current);
    }

    void TextField::key_end() {
        this->current = this->input_end;
        this->input_index = this->input.size();
        set_cursor_coords(this->current);
    }

    void TextField::key_ctrl_left_arrow() {
        int i = find_previous_space();
        this->current.x -= int(this->input_index) - i;
        this->input_index = i;
        set_cursor_coords(this->current);
    }

    void TextField::key_ctrl_right_arrow() {
        int i = find_next_space();
        this->current.x += i - int(this->input_index);
        this->input_index = i;
        set_cursor_coords(this->current);
    }
}
