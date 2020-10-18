#pragma once

const char *bool_to_char(bool v) {
    static const char *TRUE  = "true";
    static const char *FALSE = "false";
    return v ? TRUE : FALSE;
}