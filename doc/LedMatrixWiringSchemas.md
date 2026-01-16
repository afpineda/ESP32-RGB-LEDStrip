# LED matrix wiring schemas

Illustrative examples of wiring schemas for a 3x3 LED matrix
and the resulting pixel order (0 to 8).

## Top-left, rows

- Serpentine:

```text
0 1 2
5 4 3
6 7 8
```

- Linear:

```text
0 1 2
3 4 5
6 7 8
```

## Top-left, columns

- Serpentine:

```text
0 5 6
1 4 7
2 3 8
```

- Linear:

```text
0 3 6
1 4 7
2 5 8
```

## Top-right, rows

- Serpentine:

```text
2 1 0
3 4 5
8 7 6
```

- Linear:

```text
2 1 0
5 4 3
8 7 6
```

## Top-right, columns

- Serpentine:

```text
6 5 0
7 4 1
8 3 2
```

- Linear:

```text
6 3 0
7 4 1
8 5 2
```

## Bottom-left, rows

- Serpentine:

```text
6 7 8
5 4 3
0 1 2
```

- Linear:

```text
6 7 8
3 4 5
0 1 2
```

## Bottom-left, columns

- Serpentine:

```text
2 3 8
1 4 7
0 5 6
```

- Linear:

```text
2 5 8
1 4 7
0 3 6
```

## Bottom-right, rows

- Serpentine:

```text
8 7 6
3 4 5
2 1 0
```

- Linear:

```text
8 7 6
5 4 3
2 1 0
```

## Bottom-right, columns

- Serpentine:

```text
8 3 2
7 4 1
6 5 0
```

- Linear:

```text
8 5 2
7 4 1
6 3 0
```
