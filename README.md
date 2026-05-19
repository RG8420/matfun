# matfun

A command-line matrix calculator for Linux written in C. Perform matrix operations, vector calculations, and matrix factorization with an easy-to-use syntax.

## Features

- **Matrix Operations**: Addition, subtraction, multiplication, element-wise product
- **Matrix-Vector Operations**: Matrix-vector and vector-matrix multiplication
- **Scalar Operations**: Scale and scalar addition
- **Unary Operations**: Transpose, inverse, determinant, trace, rank, adjugate
- **Factorization**: LU, QR, Cholesky, Eigenvalue, SVD
- **Vector Operations**: Dot product, cross product, magnitude, normalization
- **Element-wise Math**: sin, cos, exp, log, abs, sqrt
- **Matrix Construction**: Identity, zeros, ones, diagonal
- **Norm Calculations**: Frobenius, L1, Infinity

## Quick Install

```bash
curl -sSL https://raw.githubusercontent.com/RG8420/matfun/main/install.sh | bash
```

Then open a new terminal and type:
```bash
matfun
```

## Manual Install

```bash
git clone https://github.com/RG8420/matfun.git
cd matfun
make
sudo make install
```

For user-level installation (no sudo):
```bash
make install PREFIX=$HOME/.local
```

## Usage

### Create Matrices

```
matfun> A = [[1, 2], [3, 4]]
matfun> B = [[5, 6], [7, 8]]
matfun> v = [1, 2, 3]
```

### Basic Operations

```
matfun> C = add A B        # Matrix addition
matfun> C = mult A B       # Matrix multiplication
matfun> C = emult A B      # Element-wise multiply
```

### Unary Operations

```
matfun> invA = inverse A   # Matrix inverse
matfun> det A              # Determinant (prints result)
matfun> tr = trace A       # Trace
matfun> rk = rank A        # Rank
matfun> At = transpose A   # Transpose
```

### Matrix-Vector

```
matfun> result = mvmult A v # Matrix * vector
matfun> result = vmult v A # Vector * matrix
```

### Factorization

```
matfun> lu A               # LU decomposition (prints L and U)
matfun> qr A               # QR decomposition
matfun> cholesky A         # Cholesky (SPD matrices)
matfun> eig A              # Eigenvalues/vectors
matfun> svd A              # SVD
```

### Vector Operations

```
matfun> d = dot v1 v2      # Dot product
matfun> c = cross v1 v2    # Cross product
matfun> m = magnitude v    # Vector magnitude
matfun> n = normalize v    # Unit vector
```

### Element-wise Math

```
matfun> sin A, cos A, exp A, log A, abs A, sqrt A
```

### Construction

```
matfun> I = identity 3     # 3x3 identity
matfun> Z = zeros 3 4      # 3x4 zero matrix
matfun> O = ones 2 3       # 2x3 matrix of ones
matfun> D = diag [1,2,3]   # Diagonal matrix
```

### Norms

```
matfun> frobenius A        # Frobenius norm
matfun> norm1 A            # L1 norm
matfun> norminf A          # Infinity norm
```

### Commands

```
matfun> list               # List all stored matrices
matfun> clear              # Clear all matrices
matfun> help               # Show help
matfun> quit               # Exit
```

## Operations Reference

| Command | Description |
|---------|-------------|
| `add A B` | A + B |
| `sub A B` | A - B |
| `mult A B` | Matrix product |
| `emult A B` | Hadamard product |
| `scale A k` | k * A |
| `sadd A k` | A + k |
| `transpose A` | A^T |
| `inverse A` | A^-1 |
| `det A` | Determinant |
| `trace A` | Trace |
| `rank A` | Rank |
| `adjugate A` | Adjugate |
| `mvmult A v` | Matrix * vector |
| `vmult v A` | Vector * matrix |
| `dot v1 v2` | Dot product |
| `cross v1 v2` | Cross product |
| `magnitude v` | Vector magnitude |
| `normalize v` | Unit vector |
| `identity n` | n×n identity |
| `zeros m n` | m×n zeros |
| `ones m n` | m×n ones |
| `diag [v]` | Diagonal matrix |
| `lu A` | LU decomposition |
| `qr A` | QR decomposition |
| `cholesky A` | Cholesky |
| `eig A` | Eigen decomposition |
| `svd A` | SVD |
| `frobenius A` | Frobenius norm |
| `norm1 A` | L1 norm |
| `norminf A` | Infinity norm |

## Makefile Targets

```bash
make           # Build
make clean     # Clean
make install   # Install (sudo required)
make uninstall # Uninstall
make test      # Test
make help      # Help
```

## Requirements

- GCC (tested with GCC 11+)
- Linux/Unix system
- GNU Make

## Uninstall

```bash
bash install.sh --uninstall
```

Or manually:
```bash
sudo make uninstall
```

## License

MIT License - See [LICENSE](LICENSE) file.

## Author

Rahul Golder