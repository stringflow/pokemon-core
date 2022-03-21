#ifndef UTILITY_H
#define UTILITY_H
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(val) ((val) < 0 ? -(val) : (val))
#define CLAMP(val, low, high) (MIN((high), MAX((low), (val))))

// NOTE(stringflow): https://stackoverflow.com/a/46711735/7281499
constexpr u64 hash(const char *src, int off = 0) {
    return !src[off] ? 5381 : (hash(src, off+1)*33) ^ src[off]; 
}

int diff8(int old_value, int new_value) {
    int forwards = (int) ((new_value - old_value) & 0xff);
    int backwards = (int) ((old_value - new_value) & 0xff);
    return forwards <= 128 ? forwards : -backwards;
}

void endian_swap(u32 *value) {
    *value = ((*value << 24) | 
              ((*value & 0xff00) << 8) |
              ((*value >> 8) & 0xff00) |
              (*value >> 24));
}

bool in_array(int *array, int length, int value) {
    for(int i = 0; i < length; i++) {
        if(array[i] == value) return true;
    }
    return false;
}

bool is_alpha(char c) {
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

bool is_number(char c) {
    return c >= '0' && c <= '9';
}

bool is_hex(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
}

int get_hex(char c) {
    int result = -1;
    
    if(is_number(c)) {
        result = c - '0';
    } else if(c >= 'A' && c <= 'F') {
        result = 0xA + (c - 'A');
    } else if(c >= 'a' && c <= 'f') {
        result = 0xa + (c - 'a');
    }
    
    return result;
}

int ascii_to_int(const char *string, int base, int *size = nullptr) {
    int result = 0;
    bool is_negative = false;
    
    const char *at = string;
    if(at[0] == '+') {
        at++;
    } else if(at[0] == '-') {
        is_negative = true;
        at++;
    }
    
    while(at[0]) {
        int digit = get_hex(at[0]);
        if(digit >= base || digit < 0) break;
        
        result = result * base + digit;
        at++;
    }
    
    if(size) *size = at - string;
    return is_negative ? -result : result;
}

int copy_string(char *dest, const char *src) {
    int copied = 0;
    while(src[0]) {
        *dest++ = *src++;
        copied++;
    }
    
    *dest = 0;
    return copied;
}

int copy_string_until(char *dest, const char *src, char terminator) {
    int copied = 0;
    while(src[0] && src[0] != terminator) {
        *dest++ = *src++;
        copied++;
    }
    
    *dest = 0;
    return copied;
}

const char * find_character(const char *string, char target) {
    const char *result = string;
    while(result[0] && result[0] != target) result++;
    
    return result[0] ? result : nullptr;
}

const char * get_file_extension(const char *file_name) {
    const char *dot = find_character(file_name, '.');
    if(!dot) return "";
    return dot;
}

// TODO(stringflow): Could make this work without a string copy
struct StringIterator {
    const char *whole_string;
    char current_element[64];
    int offset;
};

bool iterate_over_path(StringIterator *it) {
    char *dest = it->current_element;
    char current_char;
    while((current_char = it->whole_string[it->offset])) {
        it->offset++;
        if(current_char == ' ') continue;
        
        *dest++ = current_char;
        *dest = 0;
        switch(hash(it->current_element)) {
            case hash("A"): {
                // NOTE(stringflow): if next character in string is a plus, it's a gen2
                // action, so don't return just yet.
                if(it->whole_string[it->offset] != '+') {
                    return true;
                }
            } break;
            case hash("R"):
            case hash("L"):
            case hash("U"):
            case hash("D"):
            case hash("A+R"):
            case hash("A+L"):
            case hash("A+U"):
            case hash("A+D"):
            case hash("S_B"):
            case hash("S_A_B_S"):
            case hash("S_A_B_A_B_S"):
            case hash("SEL"): {
                return true;
            } break;
        }
    }
    
    return false;
}

bool next_element(StringIterator *it, char delimiter) {
    int copied = copy_string_until(it->current_element, it->whole_string + it->offset, delimiter);
    if(copied == 0) return false;
    
    it->current_element[copied] = 0;
    it->offset += copied; 
    // NOTE(stringflow): skip past the delimiter only if null terminator hasn't been reached
    if(it->whole_string[it->offset]) {
        it->offset++;
    }
    
    return true;
}

struct EntireFile {
    u8 *data;
    int size;
};

EntireFile read_entire_file(const char *filename) {
    EntireFile result = {};
    
    FILE *file = fopen(filename, "rb");
    if(file) {
        fseek(file, 0, SEEK_END);
        result.size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        if(result.size) {
            result.data = new u8[result.size];
            fread(result.data, 1, result.size, file);
        }
        
        fclose(file);
    }
    
    return result;
}

void free_entire_file(EntireFile *file) {
    if(file->data) {
        delete[] file->data;
        file->data = nullptr;
    }
    
    file->size = 0;
}

bool write_entire_file(const char *filename, const void *data, int size) {
    FILE *file = fopen(filename, "wb");
    if(file) {
        fwrite(data, 1, size, file);
        fclose(file);
        return true;
    }
    
    return false;
}

#endif //UTILITY_H
