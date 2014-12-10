Name: Elmer landaverde
PID: 9054-91691

Description:
This prologe program supports the following queries.

sister(X, Y)        -> Where X is the sister of Y
brother(X, Y)       -> Where X is the brother of Y
sibling(X, Y)       -> Where X is the sibling of Y
mother(X, Y)        -> Where X is the mother of Y
father(X, Y)        -> Where X is the father of Y
husband(X, Y)       -> Where X is the husband of Y
parent(X, Y)        -> Where X is the parent of Y
wife(X, Y)          -> Where X is the wife of Y
spouse(X, Y)        -> Where X is the spouse of Y
grandfather(X, Y)   -> Where X is the grandfather of Y
grandmother(X, Y)   -> Where X is the grandmother of Y
brother_inlaw(X, Y) -> Where X is the brother_inlaw of Y
sister_inlaw(X, Y)  -> Where X is the sister_inlaw of Y
sibling_inlaw(X, Y) -> Where X is the sibling_inlaw of Y
aunt(X, Y)          -> Where X is the aunt of Y
uncle(X, Y)         -> Where X is the uncle of Y
greatuncle(X, Y)    -> Where X is the greatuncle of Y
greataunt(X, Y)     -> Where X is the greataunt of Y

All of these rules and facts are stored in the "consult.pl" file. This file is imported into "input.pl" by means of the consult command. The input file has only one query that gets the greataunt of Harry which returns Dianna.
