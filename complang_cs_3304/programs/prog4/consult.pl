sister(alice, claude).
sister(florence, edward).
sister(florence, grace).
sister(grace, florence).
sister(grace, edward).

brother(claude, alice).
brother(edward, florence).
brother(edward, grace).

husband(claude, dianna).
husband(bob, alice).
husband(keith, grace).
husband(edward, jennifer).

father(bob, edward).
father(bob, florence).
father(bob, grace).
father(edward, iona).
father(keith, harry).

%% Siblings

sibling(X, Y) :- brother(X, Y); sister(X, Y).

%%  Parents

mother(Mother, Child) :- husband(Husband, Mother), father(Husband, Child).

parent(Parent, Child) :- mother(Parent, Child); father(Parent, Child).

wife(Wife, Husband) :- husband(Husband, Wife).

spouse(X, Y) :- husband(X, Y); wife(X, Y).

grandfather(Grandfather, Child) :- father(Grandfather, Parent), parent(Parent, Child).

grandmother(Grandmother, Child) :- mother(Grandmother, Parent), parent(Parent, Child).

%% Inlaws

brother_inlaw(Inlaw, Person) :- (sister(Sister, Person), husband(Inlaw, Sister)); (spouse(Spouse, Person), brother(Inlaw, Spouse)).

sister_inlaw(Inlaw, Person) :- (brother(Brother, Person), husband(Brother, Inlaw)); (spouse(Spouse, Person), sister(Inlaw, Spouse)).

sibling_inlaw(Inlaw, Person) :- sister_inlaw(Inlaw, Person); brother_inlaw(Inlaw, Person).

%% Aunt/Uncles

aunt(Aunt, Child) :- parent(Parent, Child), (sister(Aunt, Parent); sister_inlaw(Aunt, Parent)).

uncle(Uncle, Child) :- parent(Parent, Child), (brother(Uncle, Parent); brother_inlaw(Uncle, Parent)).

greatuncle(Greatuncle, Child) :- parent(Parent, Child), uncle(Greatuncle, Parent).

greataunt(Greataunt, Child) :- parent(Parent, Child), aunt(Greataunt, Parent).
