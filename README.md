# ulopcodes

Allows emitting opcodes from PHP userland.

[![Build Status](https://travis-ci.org/pmmaga/php-ulopcodes.svg?branch=master)](https://travis-ci.org/pmmaga/php-ulopcodes)

Currently supports PHP 7.1

## Usage

You can use the function `[$result = ]ulopcodes_emit($opcode[, $op1][, $op2])` to emit an opcode into the current oparray. Optionally, you can set the operands and assign the result.

### Example

Here's an Hello World:
```
<?php

function getGreeting() {
    ulopcodes_emit(ZEND_RETURN, "Hello world!");
}

ulopcodes_emit(ZEND_INIT_FCALL, IS_UNUSED, 'getgreeting');
$greeting = ulopcodes_emit(ZEND_DO_FCALL);

ulopcodes_emit(ZEND_ECHO, $greeting);
```
## Installation

As it is a Zend Extension, it must be built as a `shared` extension.
Remember to add the `zend_extension` entry to your ini file.

## Configuration

The following settings are supported:
```
ulopcodes.enabled = false ; Enable to activate ulopcodes_emit
ulopcodes.dump_oparray = false ; Enable to print a dump of the oparrays after modification
```
