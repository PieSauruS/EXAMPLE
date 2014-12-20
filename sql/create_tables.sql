DROP TABLE USER_FILTERS;
DROP TABLE USER_LIST_CONTAINS;
DROP TABLE USR;
DROP TABLE USER_LIST;


/*allow players to block people or add people
each player(usr) will get two list ids:
1) for blocking
2) for contacts
*/
CREATE TABLE USER_LIST(
    list_id serial,
    list_type char(10) NOT NULL,
    PRIMARY KEY(list_id)
);

/*the actual player along with his personal informations*/
CREATE TABLE USR(
    login char(50) UNIQUE,
    password char(50) NOT NULL,
    status char(140),
    block_list integer,
    contact_list integer,
    hours_played integer,
    games_played integer,
    pos_commend integer,
    neg_commend integer,
    PRIMARY KEY(login),
    FOREIGN KEY(block_list) REFERENCES USER_LIST(list_id),
    FOREIGN KEY(contact_list) REFERENCES USER_LIST(list_id)
);

/* people on the contact list will be associated*/
/* a weak constraint: blocked or contact with their name*/
CREATE TABLE USER_LIST_CONTAINS(
    list_id integer,
    list_member char(50),
    PRIMARY KEY(list_id, list_member),
    FOREIGN KEY(list_id) REFERENCES USER_LIST(list_id) ON DELETE CASCADE,
    FOREIGN KEY(list_member) REFERENCES USR(login) ON DELETE CASCADE
);

CREATE TABLE USER_FILTERS(
    username char(50), 
    hours_played integer,
    games_played integer,
    pos_commend integer,
    neg_commend integer,
    PRIMARY KEY(username),
    FOREIGN KEY(username) REFERENCES USR(login) ON DELETE CASCADE
);
