using System;
using System.IO;

namespace Profiler.Data
{
    public abstract class Frame
    {
        public Stream BaseStream { get; private set; }

        public virtual String Description { get; set; }
        public virtual double Duration { get; set; }

        public bool IsLoaded { get; protected set; }
        public abstract void Load();

        public Frame(Stream baseStream)
        {
            BaseStream = baseStream;
        }

        public abstract DataResponse.Type ResponseType { get; }
    }
}
