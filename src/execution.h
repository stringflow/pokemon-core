#ifndef EXECUTION_H
#define EXECUTION_H

struct Action {
    char string[32];
    int repetitions;
};

Action parse_action(const char *action) {
    Action result = {};
    
    char *dest = result.string;
    while(action[0]) {
        if(is_number(*action)) {
            int size;
            result.repetitions = ascii_to_int(action, 10, &size);
            action += size;
        } else {
            *dest++ = *action++;
        }
    }
    
    return result;
}

void rby_injectinput(GameBoy *, int);
void rby_press(GameBoy *, int);
int rby_execute(GameBoy *, const char *);
void rby_cleartext(GameBoy *, int);
void rby_pickupitem(GameBoy *);

void gsc_injectinput(GameBoy *, int);
void gsc_press(GameBoy *, int);
int gsc_execute(GameBoy *, const char *);
void gsc_cleartext(GameBoy *, int);
void gsc_pickupitem(GameBoy *);

#endif //EXECUTION_H
