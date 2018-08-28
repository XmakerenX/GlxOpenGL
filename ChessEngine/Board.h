#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <string>
#include <boost/signals2/signal.hpp>
#include <boost/bind/bind.hpp>
#undef min
#undef max
#include <fstream>
#include <vector>

#include "Chess.h"
#include "Piece.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"
#include "Queen.h"

class king;

enum COLORS{BLACK = -1,WHITE = 1};
enum SIDES {UPPER, BOTTOM};

class board
{
public:
    typedef boost::signals2::signal<void (piece*)>       singal_pieceCreated;
    typedef boost::signals2::signal<void (std::string)>  singal_gameover;
    typedef boost::signals2::signal<void (int)>          signal_endTurn;

    //-------------------------------------------------------------------------
    // Constructors & Destructors for This Class.
    //-------------------------------------------------------------------------
    board();
    ~board(void);
    
    void init();
    
    piece*      createPiece(int playerColor, PIECES pieceType, BOARD_POINT piecePos);
    bool        createStartingPiece(int i, int j, int playerColor);

    void        connectToPieceCreated   (const singal_pieceCreated::slot_type& subscriber);
    void        connectToGameOver       (const singal_gameover::slot_type& subscriber);
    void        conntectToEndTurn       (const signal_endTurn::slot_type& subscriber );

    //BOARD_POINT getPieceSquare(piece * pPiece);
    void        killPiece(piece * pPieceToKill, BOARD_POINT pieceSquare);
    void        endTurn();
    void        reverseMove();
    bool        resetGame();

    void        SaveBoardToFile();
    bool        LoadBoardFromFile();

    //-------------------------------------------------------------------------
    // Functions that control the pawn movement logic
    //-------------------------------------------------------------------------
    void processPress(BOARD_POINT pressedSqaure);
    bool PromoteUnit (PIECES type);
    bool validateMove(BOARD_POINT startLoc,BOARD_POINT newLoc);
    bool castling(int dx , int dy);

    //-------------------------------------------------------------------------
    // Functions that detect if the game has come to an end
    //-------------------------------------------------------------------------
    bool isEndGame(int curretPlayer);
    bool isDraw(int currentPlayer);


    //-------------------------------------------------------------------------
    // Functions that help detect if the king is in threat
    //-------------------------------------------------------------------------
    bool isKingInThreat(int player,bool getAllAttackers);
    bool validateKingThreat(int curretPlayer);

    //-------------------------------------------------------------------------
    // Functions that detect if pawns can move or is it stuck in his place
    //-------------------------------------------------------------------------
    bool canPawnMove	(BOARD_POINT pieceSqaure,int color,int curretPlayer);
    void markPawnMoves	(BOARD_POINT pieceSqaure,int color,int curretPlayer);
    void ScanPawnMoves	(BOARD_POINT pieceSqaure,int color,int curretPlayer, DIR_VEC dir);

    //-------------------------------------------------------------------------
    // Get methods for this class
    //-------------------------------------------------------------------------
    std::string getBoardStatus () const;
    bool        isBoardActive  () const;
    bool        getKingThreat  () const;
    bool        isUnitPromotion() const;

private:
    //the actual game board indicate what piece on what square
    piece * SBoard[boardY][boardX];

    //the pawn that currently  being moved
    piece * currentPawn;
    //used to save the piece on the target square in order to reverse if the move is not valid
    piece * prevPawn;

    //the square that was pressed
    BOARD_POINT  startSquare;
    // the square that the piece should be moved to 
    BOARD_POINT  targetSqaure;

    std::vector<piece*> pawnsVec[2];
    std::vector<piece*> deadPawnsVec[2];

    king * kings[2];
    int attackers;

    BOARD_POINT attLoc;

    unsigned int pieceCount;

    int currentPlayer;
    bool m_gameActive;
    bool m_unitPromotion;
    bool m_kingInThreat;
    std::string m_curStatus;

    boost::signals2::signal<void (piece*)>  m_pieceCreatedSig;
    boost::signals2::signal<void (std::string)> m_gameOverSig;
    boost::signals2::signal<void (int)>         m_endTurnSig;
    
};
#endif // _BOARD_H_ 