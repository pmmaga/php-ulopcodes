# ulopcodes

Allows emitting opcodes from PHP userland.

[![Build Status](https://travis-ci.org/pmmaga/php-ulopcodes.svg?branch=master)](https://travis-ci.org/pmmaga/php-ulopcodes)

Supports PHP 7.1+

## Usage

You can use the function `[$result = ]ulopcodes_emit($opcode[, $op1][, $op2])` to emit an opcode into the current oparray. Optionally, you can set the operands and assign the result.

### Example

Here's an Hello World:
```
<?php

function getGreeting($name) {
    $greeting = ulopcodes_emit(ZEND_CONCAT, "Hello ", $name);
    $greeting = ulopcodes_emit(ZEND_CONCAT, $greeting, "!\n");
    ulopcodes_emit(ZEND_RETURN, $greeting);
}

ulopcodes_emit(ZEND_ASSIGN, $fullGreeting, getGreeting("world"));
ulopcodes_emit(ZEND_ECHO, $fullGreeting);
```
This will generate the following oparrays:
> Printed when your run with the ini setting `ulopcodes.dump_oparray = 1`

```
--- Function: getGreeting ---
0 - Opcode: ZEND_RECV Op1: (8 - 1) Op2: (8 - -1) Ext: 0 Res: (16 - 48)
...
5 - Opcode: ZEND_CONCAT Op1: (1 - 2) Op2: (16 - 48) Ext: 0 Res: (4 - 0)
6 - Opcode: ZEND_ASSIGN Op1: (16 - 64) Op2: (4 - 0) Ext: 0 Res: (8 - 1)
...
11 - Opcode: ZEND_CONCAT Op1: (16 - 64) Op2: (1 - 5) Ext: 0 Res: (4 - 2)
12 - Opcode: ZEND_ASSIGN Op1: (16 - 64) Op2: (4 - 2) Ext: 0 Res: (8 - 3)
...
16 - Opcode: ZEND_RETURN Op1: (16 - 64) Op2: (8 - 0) Ext: 0 Res: (8 - 4)
17 - Opcode: ZEND_RETURN Op1: (1 - 8) Op2: (8 - 0) Ext: -1 Res: (8 - 0)
--- End function ---
--- Function: (no name) ---
...
4 - Opcode: ZEND_INIT_FCALL Op1: (8 - 160) Op2: (1 - 2) Ext: 1 Res: (8 - 0)
5 - Opcode: ZEND_SEND_VAL Op1: (1 - 3) Op2: (8 - 1) Ext: 0 Res: (8 - 48)
6 - Opcode: ZEND_DO_UCALL Op1: (8 - 0) Op2: (8 - 0) Ext: 0 Res: (4 - 0)
7 - Opcode: ZEND_NOP Op1: (8 - 0) Op2: (8 - 0) Ext: 0 Res: (8 - 0)
8 - Opcode: ZEND_ASSIGN Op1: (16 - 48) Op2: (4 - 0) Ext: 0 Res: (8 - 1)
...
12 - Opcode: ZEND_ECHO Op1: (16 - 48) Op2: (8 - 0) Ext: 0 Res: (8 - 2)
13 - Opcode: ZEND_RETURN Op1: (1 - 6) Op2: (8 - 0) Ext: -1 Res: (8 - 0)
--- End function ---
```
And produce the expect output of the `ZEND_ECHO` call:
```
Hello world!
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
