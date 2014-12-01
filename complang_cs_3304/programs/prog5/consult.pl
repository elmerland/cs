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
father(edward, iona).
father(keith, harry).

is_sibling(X, Y) :- brother(X, Y); sister(X, Y).

%%  Parents

is_father(Father, Child) :- father(Father, Child); (father(Father, Child2), is_sibling(Child2, Child)).

is_mother(Mother, Child) :- husband(Husband, Mother), is_father(Husband, Child).

is_parent(Parent, Child) :- is_mother(Parent, Child); is_father(Parent, Child).

is_wife(Wife, Husband) :- husband(Husband, Wife).

is_spouse(X, Y) :- husband(X, Y); is_wife(X, Y).

is_grandfather(Grandfather, Child) :- is_father(Grandfather, Parent), is_parent(Parent, Child).

is_grandmother(Grandmother, Child) :- is_mother(Grandmother, Parent), is_parent(Parent, Child).

%% Inlaws

is_brother_inlaw(Inlaw, Me) :- (sister(Sister, Me), husband(Inlaw, Sister)); (is_spouse(Spouse, Me), brother(Inlaw, Spouse)).

is_sister_inlaw(Inlaw, Me) :- (brother(Brother, Me), husband(Brother, Inlaw)); (is_spouse(Spouse, Me), sister(Inlaw, Spouse)).

is_sibling_inlaw(Inlaw, Me) :- is_sister_inlaw(Inlaw, Me); is_brother_inlaw(Inlaw, Me).

%% Aunt/Uncles

is_aunt(Aunt, Child) :- is_parent(Parent, Child), (sister(Aunt, Parent); is_sister_inlaw(Aunt, Parent)).

is_uncle(Uncle, Child) :- is_parent(Parent, Child), (brother(Uncle, Parent); is_brother_inlaw(Uncle, Parent)).

is_greatuncle(Greatuncle, Child) :- is_parent(Parent, Child), is_uncle(Greatuncle, Parent).

is_greataunt(Greataunt, Child) :- is_parent(Parent, Child), is_aunt(Greataunt, Parent).
