enum kindOfToken {
	END_OF_INPUT = 0,   // defined by flex system
	LPAREN,	RPAREN,
	PLUS,	MINUS,	TIMES,
	BOOL_LITERAL,	         // 2021S
	EQUAL, LESS_OR_EQUAL, GREATER_OR_EQUAL,  // 2021S
	IF, LET_STAR,
	INT_LITERAL, IDENTIFIER
};

void scannerError(); /// define in whatever uses the regexp-based scanner

extern int tokenCount;  // not static because the .l file needs it too
