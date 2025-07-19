#maintainer Rafael de Sousa - el-rafa-dev

pkgname=merge-aur
maintainer=el-rafa-dev
pkgver=1.1
pkgrel=2
pkgdesc="A minimal and easy AUR helper written in C++"
arch=('any')
url="https://github.com/el-rafa-dev/merge-aur"
license=('MIT')
depends=('curl' 'git')

source=("https://github.com/el-rafa-dev/merge-aur/releases/download/v1.2-2/merge.1.2-2.zip")
sha256sums=('2b26976196f9fd1631889f4a5e2c7a069520cc745bc4845a0e9eb083e32004b0')

build(){
  g++ -std=c++17 src/main.cc -o merge -lcurl -lstdc++fs -I.
}

package() {
  cd "${srcdir}"
  install -Dm755 merge "${pkgdir}/usr/bin/merge"
  install -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE" 
}