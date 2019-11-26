$Source = $args[0]
$Program = $Source.Replace(".pas", ".exe")

fpc $Source
if ($LASTEXITCODE -eq 0) {
	& $Program
}

Exit $LASTEXITCODE