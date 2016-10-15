# ulopcodes

Allows emitting opcodes from the userland

## Usage

You can use the function `[$result = ]ulopcodes_emit($opcode[, $op1][, $op2])` to emit an opcode into the current oparray. Optionally, you can set the operands and assign the result.

### Example

```
<?php

function getGreeting() {
    ulopcodes_emit(ZEND_RETURN, "Hello world!");
}

$greeting = getGreeting();

ulopcodes_emit(ZEND_ECHO, $greeting);
```

## Installation

As it is a Zend Extension, it must be built as a `shared` extension.
Remember to add the `zend_extension` entry to your ini file.
