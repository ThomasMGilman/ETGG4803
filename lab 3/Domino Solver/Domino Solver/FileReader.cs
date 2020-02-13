using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

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
    public FileReader(string fileName, string path = null)
    {   
        this.fileText = System.IO.File.ReadAllText(path == null ? fileName : path + fileName);
        foreach (string line in this.fileText.Split('\n'))
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
