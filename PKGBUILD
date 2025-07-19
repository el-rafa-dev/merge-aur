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
sha256sums=('65ad1982778b35b491ec03d88caf7fbc1bf51c566ed8ec9a7b2766023da4b71b')

build(){
  unzip merge.${pkgver}-${pkgrel}.zip
  g++ -std=c++17 src/main.cc -o merge -lcurl -lstdc++fs -I.
}

package() {
  cd "${srcdir}"
  install -Dm755 merge "${pkgdir}/usr/bin/merge"
  install -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE" 
}