SELECT  DIR_GUID,
        DIR_PARENT_UUID,
        DIR_NAME,
        DIR_DESCRIPTION,
        DIR_ICON,
        DIR_ORDER,
        DT_MODIFIED,
        MF_VERSION
FROM
   MF_DIR
WHERE DIR_GUID='%1'
