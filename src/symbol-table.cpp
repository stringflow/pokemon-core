int symbol_lookup(GameBoy *gb, const char *label) {
    switch(gb->game) {
        case RED: return symbol_lookup_red(label);
        case BLUE: return symbol_lookup_blue(label);
        case YELLOW: return symbol_lookup_yellow(label);
        case GOLD: return symbol_lookup_gold(label);
        case SILVER: return symbol_lookup_silver(label);
        case CRYSTAL: return symbol_lookup_crystal(label);
        default: return 0;
    }
}

int translate_symbol_label(GameBoy *gb, const char *label) {
    char base_label[64];
    int offset = 0;
    
    char *dest = base_label;
    const char *src = label;
    
    // NOTE(stringflow): copy until either '+', '-', or null terminator, optionally parse 
    // offset.
    while(src[0]) {
        if(src[0] == '+' || src[0] == '-') {
            offset = ascii_to_int(src, 16);
            break;
        }
        *dest++ = *src++;
    }
    
    *dest = 0;
    
    return symbol_lookup(gb, base_label) + offset;
}