#ifndef TOKENIZER_H_
#define TOKENIZER_H_


/// A string tokenizer
class Tokenizer
{
public:
	Tokenizer();
	~Tokenizer();

	/// Sets the string that will be used for the tokenization.
	void setString(const char *c,const char *_delim);

	/// Returns a token. If there are no tokens left, the function 
	///  will return NULL.
	char* getToken(char *sbuf,int len);
protected:
	char *strbuf;
	char *delim;
	char *current;
	int len;
	int tokens;
	int dlen;

	int isDelimiter(char c);
};

#endif // TOKENIZER_H_
