for (checkPos.y = 0; checkPos.y < FIELD_HEIGHT; checkPos.y ++)
{
    char shortTemp[10] = "";
    char temp[256] = "";
    for (checkPos.x = 0; checkPos.x < FIELD_WIDTH; checkPos.x ++)
    {
        sprintf(shortTemp, "%i", result[checkPos.y][checkPos.x]);
        strcat(temp, shortTemp);
    }
    debugMsg(temp);
}