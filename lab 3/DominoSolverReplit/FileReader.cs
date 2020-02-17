using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

class mazeText
{
    public string maze = "# The firt item in each tuple consists of the value of the (domino pip / node)\n"+
"# The second item in the tuple is the associated node that is attached to this (domino / node) with no move cost between them\n" +
"# r:right, u:up, d:down, l:left\n" +
"start = 0:0\n" +
"end = 0:19\n" +
"maze{\n" +
"	1:d,	3:r,	2:l,	3:d,	2:r,	5:l,	5:r,	4:l,	4:d,	5:d,	5:r,	6:l,	6:d\n" +
"	2:u,	1:d,	4:d,	4:u,	2:r,	6:l,	3:r,	2:l,	1:u,	2:u,	3:r,	2:l,	2:u\n" +
"	2:d,	6:u,	5:u,	5:r,	3:l,	6:d,	3:d,	2:d,	1:d,	2:r,	5:l,	2:d,	2:d\n" +
"	3:u,	6:r,	2:l,	2:r,	3:l,	1:u,	1:u,	4:u,	3:u,	3:d,	5:d,	4:u,	3:u\n" +
"	3:r,	5:l,	5:r,	4:l,	4:r,	1:l,	1:d,	4:d,	3:d,	5:u,	1:u,	4:d,	3:d\n" +
"	3:r,	2:l,	2:r,	1:l,	6:r,	1:l,	5:u,	5:u,	2:u,	5:d,	1:d,	5:u,	4:u\n" +
"	6:d,	6:r,	4:l,	2:d,	6:d,	5:r,	6:l,	6:r,	2:l,	4:u,	6:u,	5:d,	4:d\n" +
"	1:u,	1:d,	4:d,	3:u,	2:u,	5:d,	1:r,	3:l,	3:r,	4:l,	6:d,	4:u,	2:u\n" +
"	1:d,	5:u,	2:u,	3:r,	2:l,	3:u,	3:r,	1:l,	1:d,	5:d,	5:u,	4:d,	2:d\n" +
"	4:u,	5:d,	2:r,	5:l,	5:d,	2:r,	5:l,	5:d,	2:u,	1:u,	5:d,	1:u,	6:u\n" +
"	4:d,	3:u,	4:r,	6:l,	1:u,	1:d,	3:d,	3:u,	2:r,	4:l,	4:u,	1:d,	6:d\n" +
"	5:u,	3:r,	4:l,	6:r,	3:l,	4:u,	1:u,	1:r,	6:l,	6:r,	2:l,	2:u,	3:u\n" +
"	5:r,	6:l,	6:r,	4:l,	3:d,	4:r,	3:l,	3:r,	2:l,	2:r,	1:l,	1:r,	3:l\n" +
"	5:r,	2:l,	2:d,	4:d,	1:u,	1:r,	4:l,	2:r,	6:l,	6:r,	5:l,	5:d,	3:d\n" +
"	3:d,	3:d,	5:u,	2:u,	2:r,	6:l,	4:d,	2:d,	3:r,	4:l,	4:d,	4:u,	2:u\n" +
"	5:u,	1:u,	5:r,	1:l,	1:d,	6:d,	1:u,	5:u,	3:d,	4:d,	1:u,	1:d,	2:d\n" +
"	5:d,	1:r,	4:l,	4:d,	6:u,	3:u,	3:r,	5:l,	6:u,	6:u,	2:d,	2:u,	1:u\n" +
"	6:u,	6:d,	4:d,	2:u,	2:d,	1:r,	6:l,	6:d,	6:d,	3:d,	3:u,	4:r,	1:l\n" +
"	4:d,	4:u,	5:u,	2:d,	6:u,	6:r,	4:l,	3:u,	3:u,	6:u,	5:r,	4:l,	1:d\n" +
"	1:u,	2:r,	5:l,	5:u,	5:r,	3:l,	4:r,	5:l,	5:r,	6:l,	5:r,	2:l,	2:u\n" +
"}";
    public mazeText() { }
}

class FileReader
{
    private string fileName;
    private string filePath;
    private string fileText;
    private List<string> fileLines;

    /// <summary>
    /// Reads in a file passed to the reader
    /// If a path isnt specified, it looks directly in the bin
    /// </summary>
    /// <param name="fileName"></param>
    /// <param name="path"></param>
    public FileReader(string fileName=null, string path = null)
    {
        this.fileName = fileName;
        this.filePath = path;
        if (fileName != null)
            this.fileText = System.IO.File.ReadAllText(path == null ? fileName : path + fileName);
        else
            this.fileText = new mazeText().maze;

        this.fileLines = new List<string>();
        string[] lines = this.fileText.Split('\n');
        foreach (string line in lines)
            fileLines.Add(line);
    }

    /// <summary>
    /// Get the Name of the file parsed
    /// </summary>
    /// <returns></returns>
    public string getFileName() { return this.fileName; }

    /// <summary>
    /// Get the full path of the file parsed
    /// </summary>
    /// <returns></returns>
    public string getFilePath() { return this.filePath + this.fileName; }

    /// <summary>
    /// Get the text of the file parsed as a whole string
    /// </summary>
    /// <returns></returns>
    public string getFileText() { return fileText; }

    /// <summary>
    /// Get the text of the file parsed in line format
    /// </summary>
    /// <returns></returns>
    public List<string> getFileLines() { return fileLines; }
}
