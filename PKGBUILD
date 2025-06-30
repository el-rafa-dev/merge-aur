
pkgname=merge-aur
pkgver=1.0 #
pkgrel=1
pkgdesc="A minimal and easy AUR helper written in C++"
arch=('x86_64')
url="https://github.com/el-rafa-dev/merge-aur"
license=('MIT')
depends=('curl' 'git')

source=("https://sourceforge.net/projects/merge-aur/files/merge.1.0.tar.xz")
sha256sums=('31d76340e1679da93b92f9c0a8bbf13dd7ba259d9214923ea03406e4ddc19759')

build() {
  tar -xf merge.1.0.tar.xz
  
  g++ -std=c++17 src/main.cc -o merge -lcurl -lstdc++fs -I.
}

package() {
  cd "${srcdir}"
  install -Dm755 merge "${pkgdir}/usr/bin/merge"
  #install -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE" 
}