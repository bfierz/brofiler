﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Microsoft.Win32;

namespace Profiler.Data
{

    //class SourceLine
    //{

    //}

    //class SamplingSourceLine : SourceLine
    //{
    //  public SamplingBoardItem Item
    //  {
    //    set
    //    {
    //      if (value != null)
    //      {
    //        SelfPercent = String.Format("{0}%", value.SelfPercent);
    //        Sampled = value.Sampled.ToString();
    //        Passed = value.Passed.ToString();
    //      }
    //    }
    //  }

    //  [DisplayName("%")]
    //  [ColumnWidth(50)]
    //  public String SelfPercent { get; private set; }

    //  [ColumnWidth(50)]
    //  public String Sampled { get; private set; }

    //  [ColumnWidth(50)]
    //  public String Passed { get; private set; }

    //  public int Line { get; private set; }
    //  public String Text { get; private set; }

    //  public SamplingSourceLine(String text, int line)
    //  {
    //    Text = text;
    //    Line = line;
    //  }
    //}

    public class SourceLine
    {
        public double TotalPercent { get; private set; }
        public double Total { get; private set; }
        public double SelfPercent { get; private set; }
        public double Self { get; private set; }

        public void Add(BaseTreeNode node)
        {
            TotalPercent += node.TotalPercent;
            Total += node.Duration;

            SelfPercent += node.SelfPercent;
            Self += node.SelfDuration;
        }
    }

    public class SourceViewBase
    {
        public Dictionary<int, SourceLine> Lines { get; protected set; }
        public String Text { get; protected set; }
        public FileLine SourceFile { get; protected set; }
    }

    public class SourceView<TItem, TDescription, TNode> : SourceViewBase
    where TItem : BoardItem<TDescription, TNode>, new()
        where TNode : TreeNode<TDescription>
        where TDescription : Description
    {
        public String Description { get { return SourceFile != null ? SourceFile.File : "Unknown File"; } }

        private SourceView(Board<TItem, TDescription, TNode> board, FileLine path, String text)
        {
            Lines = new Dictionary<int, SourceLine>();

            IEnumerable<TItem> boardItems = board.Where(boardItem =>
            {
                return (boardItem.Description != null &&
                boardItem.Description.Path != null &&
                boardItem.Description.Path.File == path.File);
            });

            foreach (TItem item in boardItems)
            {
                foreach (TNode node in item.Nodes)
                {
                    SourceLine line = null;
                    if (!Lines.TryGetValue(node.Description.Path.Line, out line))
                    {
                        line = new SourceLine();
                        Lines.Add(node.Description.Path.Line, line);
                    }

                    line.Add(node);
                }
            }

            SourceFile = path;
            Text = text;
        }

        public static SourceView<TItem, TDescription, TNode> Create(Board<TItem, TDescription, TNode> board, FileLine path)
        {
            if (path == null || String.IsNullOrEmpty(path.File))
                return null;

            String file = path.File;

            while (!File.Exists(file))
            {
                OpenFileDialog openFileDialog = new OpenFileDialog();

                String filter = path.ShortName;

                openFileDialog.Title = String.Format("Where can I find {0}?", filter);
                openFileDialog.ShowReadOnly = true;

                openFileDialog.FileName = filter;

                if (openFileDialog.ShowDialog() == true)
                {
                    file = openFileDialog.FileName;
                }
                else
                {
                    return null;
                }
            }

            return new SourceView<TItem, TDescription, TNode>(board, path, File.ReadAllText(file));
        }
    }
}
