#define tolower(_c)    ( isupper(_c) ? (_c)+0x20 : (_c) )
#define toupper(_c)    ( islower(_c) ? (_c)-0x20 : (_c) )

#define isascii(_c)   ( (unsigned)(_c) < 0x80 )
#define toascii(_c)   ( (_c) & 0x7f )

#define isalpha(_c)     ( (_c >= 'A' && _c <= 'Z') || (_c >= 'a' && _c <= 'z') )
#define isupper(_c)     ( _c >= 'A' && _c <= 'Z' )
#define islower(_c)     ( _c >= 'a' && _c <= 'z' )
#define isdigit(_c)     ( _c >= '0' && _c <= '9' )
#define isxdigit(_c)    ( ( _c >= '0' && _c <= '9' ) || ( _c >= 'a' && _c <= 'f' ) || ( _c >= 'A' && _c <= 'F' ) )
#define isspace(_c)     ( _c == ' ' || _c == '\t' || _c == '\n' || _c == '\r' )
#define isalnum(_c)     ( (_c >= 'A' && _c <= 'Z') || (_c >= 'a' && _c <= 'z') || (_c >= '0' && _c <= '9') )

#define ispunct(_c)     ( _c >= '!' && _c <= '/' || _c >= ':' && _c <= '@' || _c >= '[' && _c <= '`' || _c >= '{' && _c <= '~' )
#define isprint(_c)     ( _c >= ' ' && _c < 0xFF )
