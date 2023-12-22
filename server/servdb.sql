delimiter //
    create procedure complete() 
    begin 
    create table if not exists users(
        id int auto_increment primary key,
        login varchar(100) not null,
        pw_hash varchar(30) not null,
        status varchar(8) default 'offline',
        unique(login));
    alter table users auto_increment=1;
    create table if not exists rooms(
        id int auto_increment primary key,
        title varchar(30) not null,
        unique(title));
    alter table rooms auto_increment=1;
    insert into rooms(title) values('general');
    create table if not exists msgdata(
        id int auto_increment primary key,
        room_id int not null,
        sender varchar(100) not null,
        recipient varchar(100) not null,
        content text,
        received_at timestamp,
        foreign key(room_id) references rooms(id) on delete cascade on update cascade);
    alter table msgdata auto_increment=1;
    end//
delimiter ;