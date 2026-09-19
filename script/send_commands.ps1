$serverIP = "localhost"
$port = 9000
$startNumber = 0
$commandCount = 10 

$tcpClient = New-Object System.Net.Sockets.TcpClient
try {
    $tcpClient.Connect($serverIP, $port)
    
    $stream = $tcpClient.GetStream()

    $utf8NoBOM = New-Object System.Text.UTF8Encoding($false)
    $writer = New-Object System.IO.StreamWriter($stream, [System.Text.Encoding]::UTF8)
    $writer.AutoFlush = $true

    for ($i = $startNumber; $i -lt ($startNumber + $commandCount); $i++) {
        $lineToSend = "$i"
        $writer.WriteLine($lineToSend)
    }
    Write-Host "Successfully sent $($commandCount) lines of data to $($serverIP):$($port)."

} catch {
    Write-Error "Error: $($_.Exception.Message)"
} finally {
    if ($writer) { $writer.Dispose() }
    if ($stream) { $stream.Dispose() }
    if ($tcpClient) { $tcpClient.Close() }
}
