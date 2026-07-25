import { Assert } from "std/assert"
import {
  ParsingError,
  parseBool,
  parseByte,
  parseDouble,
  parseFloat,
  parseInt,
  parseLong,
} from "./index"

function assertBoolFailure(value: string, expected: ParsingError): none {
  case parseBool(value) {
    failure: Failure -> Assert.equal(failure.error, expected)
    _: Success -> Assert.fail("expected bool parsing to fail")
  }
}

function assertIntFailure(value: string, expected: ParsingError): none {
  case parseInt(value) {
    failure: Failure -> Assert.equal(failure.error, expected)
    _: Success -> Assert.fail("expected int parsing to fail")
  }
}

function assertDoubleFailure(value: string, expected: ParsingError): none {
  case parseDouble(value) {
    failure: Failure -> Assert.equal(failure.error, expected)
    _: Success -> Assert.fail("expected double parsing to fail")
  }
}

export function testParsesStrictBooleans(): none {
  Assert.equal(parseBool("true")!, true)
  Assert.equal(parseBool("false")!, false)
  assertBoolFailure("", .EmptyInput)
  assertBoolFailure("TRUE", .InvalidFormat)
  assertBoolFailure("1", .InvalidFormat)
  assertBoolFailure(" true", .InvalidFormat)
}

export function testParsesIntegerTypes(): none {
  Assert.equal(parseByte("255")!, byte(255))
  Assert.equal(parseInt("-2147483648")!, -2147483648)
  Assert.equal(parseInt("+42")!, 42)
  Assert.equal(parseLong("9223372036854775807")!, 9223372036854775807L)
}

export function testClassifiesIntegerFailures(): none {
  assertIntFailure("", .EmptyInput)
  assertIntFailure(" 42", .InvalidFormat)
  assertIntFailure("42 ", .InvalidFormat)
  assertIntFailure("4x", .InvalidFormat)
  assertIntFailure("2147483648", .Overflow)
  assertIntFailure("-2147483649", .Underflow)

  case parseByte("-1") {
    failure: Failure -> Assert.equal(failure.error, ParsingError.Underflow)
    _: Success -> Assert.fail("expected byte underflow")
  }
  case parseByte("256") {
    failure: Failure -> Assert.equal(failure.error, ParsingError.Overflow)
    _: Success -> Assert.fail("expected byte overflow")
  }
}

export function testParsesFloatingPointTypes(): none {
  Assert.approxEqual(double(parseFloat("3.5")!), 3.5)
  Assert.approxEqual(parseDouble("-1.25e2")!, -125.0)
}

export function testClassifiesFloatingPointFailures(): none {
  assertDoubleFailure("", .EmptyInput)
  assertDoubleFailure(" 1.0", .InvalidFormat)
  assertDoubleFailure("1.0x", .InvalidFormat)
  assertDoubleFailure("1e9999", .Overflow)
  assertDoubleFailure("1e-9999", .Underflow)
}
