#include "catch.hpp"
#include "clara.h"

TEST_CASE( "parseTokens" ) {
    using namespace Clara;
    Parser parser;
    
    std::vector<Parser::Token> tokens;
    
    SECTION( "empty" ) {
        parser.parseIntoTokens( "", tokens );
        REQUIRE( tokens.empty() );
    }
    
    SECTION( "short opt flag" ) {
        parser.parseIntoTokens( "-f", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "f" );
    }
    SECTION( "long opt flag" ) {
        parser.parseIntoTokens( "--flag", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::LongOpt );
        REQUIRE( tokens[0].data == "flag" );
    }
    SECTION( "short opt with arg" ) {
        parser.parseIntoTokens( "-a", tokens );
        parser.parseIntoTokens( "payload", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "a" );

        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "payload" );
    }
    SECTION( "long opt with arg" ) {
        parser.parseIntoTokens( "--arg", tokens );
        parser.parseIntoTokens( "payload", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::LongOpt );
        REQUIRE( tokens[0].data == "arg" );
        
        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "payload" );
    }
    SECTION( "short opt with arg, colon separated" ) {
        parser.parseIntoTokens( "-a:payload", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "a" );
        
        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "payload" );
    }
    SECTION( "short opt with arg, = separated" ) {
        parser.parseIntoTokens( "-a=payload", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "a" );
        
        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "payload" );
    }

    SECTION( "combined short opt flags" ) {
        parser.parseIntoTokens( "-abc", tokens );
        REQUIRE( tokens.size() == 3 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "a" );
        REQUIRE( tokens[1].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[1].data == "b" );
        REQUIRE( tokens[2].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[2].data == "c" );
    }
    
    
    SECTION( "positional" ) {
        parser.parseIntoTokens( "abc", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::Positional );
        REQUIRE( tokens[0].data == "abc" );
    }
    
    SECTION( "positional quotes" ) {
        parser.parseIntoTokens( "\"abc def\"", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::Positional );
        REQUIRE( tokens[0].data == "\"abc def\"" );
    }

    SECTION( "positional quotes 2" ) {
        parser.parseIntoTokens( "\"abc def\"", tokens );
        parser.parseIntoTokens( "\"ghi jkl\"", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::Positional );
        REQUIRE( tokens[0].data == "\"abc def\"" );
        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "\"ghi jkl\"" );
    }
    
// Forward slash is only available on Windows
#ifdef CLARA_PLATFORM_WINDOWS
    SECTION( "short opt flag, with /" ) {
        parser.parseIntoTokens( "/f", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "f" );
    }
    SECTION( "long opt with arg, with /" ) {
        parser.parseIntoTokens( "/a", tokens );
        parser.parseIntoTokens( "payload", tokens );
        REQUIRE( tokens.size() == 2 );
        REQUIRE( tokens[0].type == Parser::Token::ShortOpt );
        REQUIRE( tokens[0].data == "a" );
        
        REQUIRE( tokens[1].type == Parser::Token::Positional );
        REQUIRE( tokens[1].data == "payload" );
    }
    SECTION( "long opt flag, with /" ) {
        parser.parseIntoTokens( "/abc", tokens );
        REQUIRE( tokens.size() == 1 );
        REQUIRE( tokens[0].type == Parser::Token::LongOpt );
        REQUIRE( tokens[0].data == "abc" );
    }
#endif
}
