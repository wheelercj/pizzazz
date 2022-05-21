#include "TextField.h"

namespace pizzazz {

    std::string getline_autocompleted(
            const std::vector<std::string>& suggestions,
            std::string default_message,
            bool must_use_suggestion,
            bool case_sensitive) {
        TextField tf(suggestions, default_message, must_use_suggestion, case_sensitive);
        return tf.getline_autocompleted();
    }

    TextField::TextField(
            const std::vector<std::string>& suggestions,
            std::string default_message,
            bool must_use_suggestion,
            bool case_sensitive) {
        this->suggestions = suggestions;
        this->default_message = default_message;
        this->must_use_suggestion = must_use_suggestion;
        this->case_sensitive = case_sensitive;
        if (default_message.size()) {
            find_and_print_suggestion();
            this->latest_suggestion = default_message;
        }
    }

    std::string TextField::getline_autocompleted() {
        std::string result;
        while (true) {
            this->key = read_key();
            if (this->key == "Enter") {
                result = kp_enter();
                if (result.size())
                    return result;
            }
            else if (this->key == "Tab") {
                result = kp_tab();
                if (result.size())
                    return result;
            }
            else if (this->key.size() == 1)
                kp_char();
            else if (this->key == "Backspace"
                    && this->current.x > this->start.x
                    && this->input.size())
                kp_backspace();
            else if (this->key == "Ctrl+Backspace"
                    && this->current.x > this->start.x
                    && this->input.size())
                kp_ctrl_backspace();
            else if (this->key == "Delete"
                    && this->current.x < this->input_end.x)
                kp_delete();
            else if (this->key == "Ctrl+Delete")
                kp_ctrl_delete();
            else if (this->key == "left arrow"
                    && this->current.x > this->start.x)
                kp_left_arrow();
            else if (this->key == "right arrow"
                    && this->current.x < this->input_end.x)
                kp_right_arrow();
            else if (this->key == "up arrow")
                kp_up_arrow();
            else if (this->key == "down arrow")
                kp_down_arrow();
            else if (this->key == "Home")
                kp_home();
            else if (this->key == "End")
                kp_end();
            else if (this->key == "Ctrl+left arrow"
                    && this->input_index > 0)
                kp_ctrl_left_arrow();
            else if (this->key == "Ctrl+right arrow"
                    && this->input_index < this->input.size())
                kp_ctrl_right_arrow();
        }
    }

    void TextField::find_and_print_suggestion() {
        clear_suggestion();
        if (this->input.empty()) {
            if (this->default_message.size())
                print_suggestion(this->default_message);
            set_cursor_coords(this->current);
            return;
        }
        for (auto it = this->suggestions.begin(); it != this->suggestions.end(); it++) {
            std::string suggestion = *it;
            std::string lc_suggestion = suggestion;
            std::string lc_input = this->input;
            if (!this->case_sensitive) {
                lc_input = to_lower(lc_input);
                lc_suggestion = to_lower(lc_suggestion);
            }
            if (lc_suggestion.find(lc_input) == 0) {
                this->latest_suggestion = suggestion;
                print_suggestion(suggestion);
                break;
            }
        }
        set_cursor_coords(this->current);
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
        std::string suffix = suggestion.substr(this->input.size());
        print_styled(suffix, { Style::bright_gray });
        this->suggestion_end = get_cursor_coords();
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

    std::string TextField::kp_enter() {
        if (this->must_use_suggestion) {
            if (this->case_sensitive) {
                if (this->latest_suggestion != this->input)
                    red_flash_text();
                else {
                    clear_suggestion();
                    return this->latest_suggestion;
                }
            }
            else if (to_lower(this->latest_suggestion) != to_lower(this->input))
                red_flash_text();
            else {
                clear_suggestion();
                return this->latest_suggestion;
            }
        }
        else {
            clear_suggestion();
            return this->input;
        }
        return "";
    }

    std::string TextField::kp_tab() {
        if (this->latest_suggestion.empty()) {
            if (!this->must_use_suggestion)
                return this->input;
            red_flash_text();
            return "";
        }
        set_cursor_coords(this->start);
        std::cout << this->latest_suggestion;
        return this->latest_suggestion;
    }

    void TextField::kp_char() {
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

    void TextField::kp_backspace() {
        this->input.erase(this->input_index - 1, 1);
        backspace_chars(1);
        this->current.x -= 1;
        this->input_end.x -= 1;
        this->input_index -= 1;
        find_and_print_suggestion();
    }

    void TextField::kp_ctrl_backspace() {
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

    void TextField::kp_delete() {
        delete_chars(1);
        this->input_end.x -= 1;
        this->input.erase(this->input_index, 1);
        find_and_print_suggestion();
    }

    void TextField::kp_ctrl_delete() {
        int s = find_next_space();
        int i = int(this->input_index);
        int diff = s - i;
        delete_chars(diff);
        this->input.erase(i, diff);
        this->input_end.x -= diff;
        find_and_print_suggestion();
    }

    void TextField::kp_left_arrow() {
        this->current.x -= 1;
        this->input_index -= 1;
        set_cursor_coords(this->current);
    }

    void TextField::kp_right_arrow() {
        this->current.x += 1;
        this->input_index += 1;
        set_cursor_coords(this->current);
    }

    void TextField::kp_up_arrow() {
        // TODO
    }

    void TextField::kp_down_arrow() {
        // TODO
    }

    void TextField::kp_home() {
        this->current = this->start;
        this->input_index = 0;
        set_cursor_coords(this->current);
    }

    void TextField::kp_end() {
        this->current = this->input_end;
        this->input_index = this->input.size() - 1;
        set_cursor_coords(this->current);
    }

    void TextField::kp_ctrl_left_arrow() {
        int i = find_previous_space();
        this->current.x -= int(this->input_index) - i;
        this->input_index = i;
        set_cursor_coords(this->current);
    }

    void TextField::kp_ctrl_right_arrow() {
        int i = find_next_space();
        this->current.x += i - int(this->input_index);
        this->input_index = i;
        set_cursor_coords(this->current);
    }
}
