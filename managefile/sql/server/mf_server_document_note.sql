CREATE TABLE IF NOT EXISTS MF_DOCUMENT_NOTE
(
   NOTE_GUID                      char(36)                       not null,
   ACOUNT_GUID                    char(36)                       not null,
   DOCUMENT_GUID                  char(36)                       not null,
   NOTE_CONTENT                   varchar(768)                   not null,
   NOTE_OWNER                     char(36)                       not null,
   PAGE                           int                            default 0,
   SHEETPAGE                      varchar(150),
   MROW                           int                            default 0,
   MCOLUMN                        int                            default 0,
   DT_MODIFIED                    char(19),
   primary key (NOTE_GUID)
)