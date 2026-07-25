# std/parse

Strict parsing of primitive values from strings.

## Usage

```doof
import { parseBool, parseInt, parseDouble } from "std/parse"

enabled := try parseBool("true")
port := try parseInt("8080")
ratio := try parseDouble("0.75")
```

The parsers consume the entire input and do not trim whitespace. Boolean parsing
accepts only the lowercase strings `"true"` and `"false"`.

All functions return `Result<T, ParsingError>`. See the
[API reference](docs/API.md) for the complete surface and error semantics.

Tests can be run with `doof test parse`.
