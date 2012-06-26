SELECT  DIR_GUID,
        DIR_NAME,
        DIR_DESCRIPTION,
        DIR_ICON,
        DIR_ORDER,
        DT_MODIFIED,
        MF_VERSION,
        DELETE_FLAG
FROM
   MF_DIR
WHERE DIR_PARENT_UUID='%1' 
AND DELETE_FLAG='%2' 
ORDER BY DIR_ORDER