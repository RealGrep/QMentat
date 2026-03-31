# Maintainer: Michel Dusseault <mike dot dusseault at gmail dot com>
pkgname=qmentat
pkgver=1.0.2  # or whatever version
pkgrel=1
pkgdesc="Mental arithmetic practice application"
arch=('x86_64')
url="https://github.com/RealGrep/QMentat"
#url="http://rincewind.tower:3001/michel/QMentat"
license=('GPL3')
depends=('qt6-base' 'gmp')
makedepends=('qt6-tools')
source=('git+https://github.com/RealGrep/QMentat.git#tag=v1.0.2')
#source=("git+ssh://forgejo@rincewind.tower/michel/QMentat.git#tag=v1.0.0")
sha256sums=('SKIP')

build() {
    cmake -B build -S QMentat \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build
}
