pkgname=qmentat
pkgver=0.9.1  # or whatever version
pkgrel=1
pkgdesc="Mental arithmetic practice application"
arch=('x86_64')
#url="https://github.com/michel/qmentat"  # or forgejo URL
url="http://rincewind.tower:3001/michel/QMentat"
license=('GPL3')
depends=('qt6-base' 'gmp')
makedepends=('qt6-tools')
#source=("$pkgname-$pkgver.tar.gz")
#source=("git+ssh://forgejo@rincewind.tower:3001/michel/QMentat.git")
source=("git+ssh://forgejo@rincewind.tower/michel/QMentat.git")
#source=("git+file:///home/michel/rincewind/code/QMentat")
sha256sums=('SKIP')

build() {
    #cd "$pkgname-$pkgver"
    #cd "QMentat"
    #qmake6 QMentat.pro
    #make
    cmake -B build -S QMentat \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build
}

package() {
    #cd "QMentat"
    #make INSTALL_ROOT="$pkgdir" install
    DESTDIR="$pkgdir" cmake --install build
}
