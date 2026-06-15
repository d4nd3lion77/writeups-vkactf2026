using System.Diagnostics;
using System.IO.Compression;
using System.Reflection;
using System.Text.RegularExpressions;

internal static class Program
{
    private static readonly Regex ValueRegex = new(@"\$\d+ = ([0-7]+)", RegexOptions.Compiled);
    private const int Iterations = 128;
    private const int MaxAttempts = 5;

    private static string GetBaseDir()
    {
        var baseDir = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "MessageAGC");
        Directory.CreateDirectory(baseDir);
        return baseDir;
    }

    private static string GetPayloadDir(string baseDir)
    {
        return Path.Combine(baseDir, "payload-" + Guid.NewGuid().ToString("N"));
    }

    private static void EnsureExtracted(string payloadDir)
    {
        Directory.CreateDirectory(payloadDir);
        using var stream = Assembly.GetExecutingAssembly().GetManifestResourceStream("payload.zip")
            ?? throw new InvalidOperationException("Embedded payload.zip not found.");
        using var archive = new ZipArchive(stream, ZipArchiveMode.Read);
        foreach (var entry in archive.Entries)
        {
            var destPath = Path.Combine(payloadDir, entry.FullName);
            if (string.IsNullOrEmpty(entry.Name))
            {
                Directory.CreateDirectory(destPath);
                continue;
            }

            Directory.CreateDirectory(Path.GetDirectoryName(destPath)!);
            entry.ExtractToFile(destPath, overwrite: true);
        }
    }

    private static void TryDeletePayloadDir(string payloadDir)
    {
        for (var i = 0; i < 12; i++)
        {
            try
            {
                if (Directory.Exists(payloadDir))
                {
                    Directory.Delete(payloadDir, recursive: true);
                }
                return;
            }
            catch
            {
                Thread.Sleep(300);
            }
        }

        try
        {
            var psi = new ProcessStartInfo("cmd.exe", $"/c ping 127.0.0.1 -n 3 > nul && rmdir /s /q \"{payloadDir}\"")
            {
                UseShellExecute = false,
                CreateNoWindow = true,
            };
            Process.Start(psi);
        }
        catch
        {
        }
    }

    private static List<string> RunAgcOnce(string payloadDir)
    {
        var yaAgcExe = Path.Combine(payloadDir, "yaAGC.exe");
        var binFile = Path.Combine(payloadDir, "Message.agc.bin");
        var symFile = Path.Combine(payloadDir, "Message.agc.symtab");

        if (!File.Exists(yaAgcExe) || !File.Exists(binFile) || !File.Exists(symFile))
        {
            throw new FileNotFoundException("Missing embedded payload files.");
        }

        var psi = new ProcessStartInfo(yaAgcExe, "Message.agc.bin --symbols=Message.agc.symtab --no-resume --dump-time=0")
        {
            WorkingDirectory = payloadDir,
            UseShellExecute = false,
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            CreateNoWindow = true,
        };

        using var p = Process.Start(psi) ?? throw new InvalidOperationException("Cannot start yaAGC.exe");

        p.StandardInput.WriteLine("break SNAP");
        p.StandardInput.WriteLine("run");
        for (var i = 0; i < Iterations - 1; i++)
        {
            p.StandardInput.WriteLine("print ERRNUM");
            p.StandardInput.WriteLine("print ERRSUB");
            p.StandardInput.WriteLine("cont");
        }
        p.StandardInput.WriteLine("print ERRNUM");
        p.StandardInput.WriteLine("print ERRSUB");
        p.StandardInput.WriteLine("quit");
        p.StandardInput.Flush();
        p.StandardInput.Close();

        var stdout = p.StandardOutput.ReadToEnd();
        var stderr = p.StandardError.ReadToEnd();
        p.WaitForExit();

        var vals = new List<string>();
        using var reader = new StringReader(stdout);
        while (true)
        {
            var line = reader.ReadLine();
            if (line is null) break;
            var m = ValueRegex.Match(line);
            if (m.Success)
            {
                vals.Add(m.Groups[1].Value);
            }
        }

        if (vals.Count < Iterations * 2 && !string.IsNullOrWhiteSpace(stderr))
        {
            throw new InvalidOperationException(stderr.Trim());
        }

        return vals;
    }

    private static int Main(string[] args)
    {
        var baseDir = GetBaseDir();
        var payloadDir = GetPayloadDir(baseDir);

        try
        {
            EnsureExtracted(payloadDir);

            List<string>? vals = null;
            Exception? lastError = null;
            for (var attempt = 0; attempt < MaxAttempts; attempt++)
            {
                try
                {
                    var current = RunAgcOnce(payloadDir);
                    if (current.Count >= Iterations * 2)
                    {
                        vals = current;
                        break;
                    }
                }
                catch (Exception ex)
                {
                    lastError = ex;
                }
            }

            if (vals is null)
            {
                Console.Error.WriteLine("No AGC values parsed.");
                if (lastError is not null)
                {
                    Console.Error.WriteLine(lastError.Message);
                }
                return 3;
            }

            for (var step = 0; step < Iterations; step++)
            {
                var prog = vals[step * 2].PadLeft(2, '0');
                var noun = vals[step * 2 + 1].PadLeft(2, '0');
                Console.WriteLine($"step={step:D4}  PROG={prog}  NOUN={noun}");
            }

            return 0;
        }
        finally
        {
            TryDeletePayloadDir(payloadDir);
        }
    }
}

