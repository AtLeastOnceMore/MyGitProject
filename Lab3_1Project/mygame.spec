Name:           mygame
Version:        1.1
Release:        alt1
Summary:        A Labirynth game
Group:          Games/Arcade 

License:        GPLv3+
URL:            https://github.com/
Source0:        mygame-1.1.tar

BuildRequires:  gcc-c++
BuildRequires:  ncurses-devel
Requires:       ncurses

%description
Labirynth game v1.1.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/usr/bin
install -m 755 mygame %{buildroot}/usr/bin/mygame

%files
/usr/bin/mygame

%changelog
* Sat Mar 01 2025 Ivan Cool <vanek@mail.ru> - 1.1-1
- Bigger map
