$listener = [System.Net.HttpListener]::new()
$listener.Prefixes.Add("http://localhost:5500/")
$listener.Start()
Write-Host "Serving on http://localhost:5500"

while ($listener.IsListening) {
    try {
        $ctx = $listener.GetContext()
    } catch { break }

    $req  = $ctx.Request
    $res  = $ctx.Response
    $path = $req.Url.LocalPath
    if ($path -eq "/" -or $path -eq "") { $path = "/index.html" }

    $file = Join-Path $PSScriptRoot $path.TrimStart("/").Replace("/", [System.IO.Path]::DirectorySeparatorChar)

    if (Test-Path $file -PathType Leaf) {
        try {
            $bytes = [System.IO.File]::ReadAllBytes($file)
            $ext   = [System.IO.Path]::GetExtension($file).ToLower()
            $res.ContentType = switch ($ext) {
                ".html" { "text/html; charset=utf-8" }
                ".css"  { "text/css; charset=utf-8" }
                ".js"   { "application/javascript; charset=utf-8" }
                default { "application/octet-stream" }
            }
            $res.StatusCode     = 200
            $res.ContentLength64 = $bytes.LongLength
            $res.OutputStream.Write($bytes, 0, $bytes.Length)
        } catch {
            $res.StatusCode = 500
        }
    } else {
        $res.StatusCode = 404
    }

    try { $res.OutputStream.Close() } catch {}
}
