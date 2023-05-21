void shell()
{
    char *line;
    char buf;
    while(1)
    {
        buf = read(0, 1);
        if (buf == '\n')
        {
            if (!strcmp(line, "exec"))
            {
                printf("kek");
            }   
            line = "";
        }
    }

}