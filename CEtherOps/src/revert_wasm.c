void revert() {
    // force a machine error
    asm("unreachable");
}