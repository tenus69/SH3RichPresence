using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;

internal sealed class MemoryReader : IDisposable
{
    private const int ProcessVmRead = 0x0010;
    private const int ProcessQueryInformation = 0x0400;

    private readonly Process process;
    private readonly IntPtr processHandle;

    public MemoryReader(Process process)
    {
        this.process = process ?? throw new ArgumentNullException(nameof(process));

        processHandle = OpenProcess(
            ProcessVmRead | ProcessQueryInformation,
            inheritHandle: false,
            process.Id);

        if (processHandle == IntPtr.Zero)
        {
            throw new Win32Exception(
                Marshal.GetLastWin32Error(),
                "Could not open the Silent Hill 3 process.");
        }
    }

    public float ReadFloatFromModuleOffset(int offset)
    {
        IntPtr baseAddress = process.MainModule?.BaseAddress
            ?? throw new InvalidOperationException(
                "Could not determine the sh3.exe base address.");

        IntPtr address = IntPtr.Add(baseAddress, offset);
        byte[] buffer = new byte[sizeof(float)];

        if (!ReadProcessMemory(
                processHandle,
                address,
                buffer,
                buffer.Length,
                out IntPtr bytesRead))
        {
            throw new Win32Exception(
                Marshal.GetLastWin32Error(),
                $"Could not read memory at 0x{address.ToInt64():X}.");
        }

        if (bytesRead.ToInt64() != buffer.Length)
        {
            throw new InvalidOperationException(
                $"Expected {buffer.Length} bytes but read {bytesRead.ToInt64()}.");
        }

        return BitConverter.ToSingle(buffer, 0);
    }
public int ReadIntFromModuleOffset(int offset)
{
    IntPtr baseAddress = process.MainModule?.BaseAddress
        ?? throw new InvalidOperationException(
            "Could not determine the sh3.exe base address.");

    IntPtr address = IntPtr.Add(baseAddress, offset);
    byte[] buffer = new byte[sizeof(int)];

    if (!ReadProcessMemory(
            processHandle,
            address,
            buffer,
            buffer.Length,
            out IntPtr bytesRead))
    {
        throw new Win32Exception(
            Marshal.GetLastWin32Error(),
            $"Could not read memory at 0x{address.ToInt64():X}.");
    }

    if (bytesRead.ToInt64() != buffer.Length)
    {
        throw new InvalidOperationException(
            $"Expected {buffer.Length} bytes but read {bytesRead.ToInt64()}.");
    }

    return BitConverter.ToInt32(buffer, 0);
}
    public void Dispose()
    {
        if (processHandle != IntPtr.Zero)
        {
            CloseHandle(processHandle);
        }
    }

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr OpenProcess(
        int desiredAccess,
        bool inheritHandle,
        int processId);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool ReadProcessMemory(
        IntPtr processHandle,
        IntPtr baseAddress,
        byte[] buffer,
        int size,
        out IntPtr bytesRead);

    [DllImport("kernel32.dll")]
    private static extern bool CloseHandle(IntPtr handle);
}