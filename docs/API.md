# std/parse API

`std/parse` provides strict, locale-independent entry points for parsing
primitive values from strings.

## Functions

| Function | Result |
| --- | --- |
| `parseBool(value: string)` | `Result<bool, ParsingError>` |
| `parseByte(value: string)` | `Result<byte, ParsingError>` |
| `parseInt(value: string)` | `Result<int, ParsingError>` |
| `parseLong(value: string)` | `Result<long, ParsingError>` |
| `parseFloat(value: string)` | `Result<float, ParsingError>` |
| `parseDouble(value: string)` | `Result<double, ParsingError>` |

Each parser consumes the complete input. Leading or trailing whitespace and
trailing characters produce `InvalidFormat`. Integer parsers use decimal
notation and accept an optional sign. `parseBool` accepts exactly `"true"` or
`"false"`.

Floating-point parsing accepts the spellings supported by the platform C
runtime, including scientific notation and non-finite values.

## ParsingError

| Member | Meaning |
| --- | --- |
| `EmptyInput` | The input string is empty |
| `InvalidFormat` | The input is not a complete value of the requested type |
| `Overflow` | The value is above the requested type's supported range |
| `Underflow` | The value is below the requested type's supported range |
