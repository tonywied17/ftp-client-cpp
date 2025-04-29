class BurgerLocator
{

    //! Constructor & Initialization    ---

    /**
     * @brief Initializes the BurgerLocator class.
     * @param dataUrl URL to fetch burger location data.
     */
    constructor(dataUrl)
    {
        this.dataUrl = dataUrl;
        this.burgerData = [];
        this.geoWatchId = null;
        this.btn = document.getElementById("near-me-btn");
        this.container = document.getElementById("burger-layers");
        this.searchInput = document.getElementById("search");
        this.colors = ["var(--lettuce-color)", "var(--tomato-color)", "var(--cheese-color)"];
        this.defaultBtnText = "Find 🍔 near me!";
        this.cheesePattern = [];
        this.burgerElements = new Map();
        this.Init();
    }

    /**
     * @brief Initializes the application by fetching data and setting up event listeners.
     */
    async Init()
    {
        await this.FetchData();
        this.GenerateCheesePattern();
        this.RenderBurgers();
        this.SetupEventListeners();
    }

    /**
     * @brief Sets up event listeners for search input and geolocation button.
     */
    SetupEventListeners()
    {
        this.btn.addEventListener("click", () => this.HandleGeolocation());
        this.searchInput.addEventListener("input", () => this.FilterBurgers());
    }


    //! Data Handling   ---

    /**
     * @brief Fetches burger location data from the provided URL.
     */
    async FetchData()
    {
        try
        {
            const response = await fetch(this.dataUrl);
            console.log(response)
            if (!response.ok)
            {
                throw { status: response.status, text: response.statusText };
            }
            this.burgerData = await response.json();
        }
        catch (error)
        {
            console.error("Error loading burger data:", error);
            const statusCode = error.status || 0;
            const statusText = `<b>HTTP Error</b>\n${BurgerLocator.StatusLabels[statusCode]}` || "Unknown Error";
            this.ShowAlert(`${statusText} $${statusCode}`);
        }
    }

    /**
     * @brief Generates a random cheese pattern for styling burger elements.
     */
    GenerateCheesePattern()
    {
        this.cheesePattern = this.burgerData.map(() => Math.random() < 0.33);
    }


    //! Burger Rendering & Filtering    ---

    /**
     * @brief Renders the list of burgers in the UI.
     */
    RenderBurgers()
    {
        this.container.innerHTML = "";
        this.burgerElements.clear();

        const sortedData = this.SortBurgerData(null, null, false);
        sortedData.forEach((item, index) =>
        {
            const element = this.CreateBurgerElement(item, index, false);
            this.burgerElements.set(
                { title: item.title.toLowerCase(), location: item.location.toLowerCase() },
                element
            );
            this.container.appendChild(element);
        });
    }

    /**
     * @brief Filters the displayed burgers based on user input.
     */
    FilterBurgers()
    {
        const searchTerm = this.searchInput.value.toLowerCase();
        this.burgerElements.forEach((element, key) =>
        {
            const { title, location } = key;
            const matches = title.includes(searchTerm) || location.includes(searchTerm);
            element.style.display = matches ? "block" : "none";
        });
    }

    /**
     * @brief Creates a DOM element for a burger item.
     * @param item Burger data object.
     * @param index Index of the item in the list.
     * @param isGeolocationAllowed Whether geolocation is used for sorting.
     * @return {HTMLElement} The created burger element.
     */
    CreateBurgerElement(item, index, isGeolocationAllowed)
    {
        const distanceText = isGeolocationAllowed ? ` - ${item.distance.toFixed(1)} mi away` : "";
        const color = this.colors[index] ?? (this.cheesePattern[index] ? "var(--cheese-color)" : "var(--patty-color)");
        const isDark = color === "var(--cheese-color)";

        const wrapper = document.createElement("div");
        wrapper.classList.add("burger-wrapper");
        wrapper.style.backgroundColor = color;

        wrapper.innerHTML = `
        <a href="${item.url}" target="_blank">
            <div class="link-item ${isDark ? 'link-item-dark' : 'link-item-light'}">
                ${item.title}
            </div>
            <p class="burger-location ${isDark ? 'text-dark' : 'text-light'}">
                ${item.location}${distanceText}
            </p>
        </a>
        `;

        return wrapper;
    }


    //! Geolocation & Sorting    ---

    /**
     * @brief Handles the geolocation request to find nearby burger locations.
     */
    HandleGeolocation()
    {
        this.searchInput.value = "";
        this.FilterBurgers();
        this.btn.textContent = "📡 Locating nearest 🍔...";

        this.geoWatchId && navigator.geolocation.clearWatch(this.geoWatchId);

        return navigator.geolocation
            ? this.geoWatchId = navigator.geolocation.watchPosition(
                ({ coords }) => this.HandlePositionUpdate(coords),
                (error) => this.HandleError(error),
                { enableHighAccuracy: true, timeout: 5000, maximumAge: 0 }
            )
            : (this.ShowAlert("Geolocation is not supported or may be restricted in embedded web views."), this.ResetButtonText());
    }

    /**
     * @brief Updates burger locations based on the user's position.
     * @param latitude User's latitude.
     * @param longitude User's longitude.
     */
    HandlePositionUpdate({ latitude, longitude }) 
    {
        if (this.geoWatchId !== null) 
        {
            navigator.geolocation.clearWatch(this.geoWatchId);
            this.geoWatchId = null;
        }

        this.UpdateLocations(latitude, longitude, true);
    }

    /**
     * @brief Handles geolocation errors and displays appropriate messages.
     * @param error Geolocation error object.
     */
    HandleError(error)
    {
        const label = `<b>Geolocation Error.</b>\n${BurgerLocator.GeolocationErrors[error.code]}` || "Mystery Geolocation Error️";
        const code = error.code;
        this.ShowAlert(`${label} $${code}`);
        setTimeout(() => this.ResetButtonText(), 100);
    }

    /**
     * @brief Updates the displayed burger locations and sorts them based on geolocation.
     * @param userLat User's latitude.
     * @param userLon User's longitude.
     * @param isGeolocationAllowed Whether geolocation sorting is enabled.
     */
    UpdateLocations(userLat = null, userLon = null, isGeolocationAllowed = false)
    {
        this.ResetButtonText();
        if (!this.burgerData.length) return;

        this.container.innerHTML = "";
        this.burgerElements.clear();

        const sortedData = this.SortBurgerData(userLat, userLon, isGeolocationAllowed);

        sortedData.forEach((item, index) =>
        {
            const element = this.CreateBurgerElement(item, index, isGeolocationAllowed);
            this.burgerElements.set(
                { title: item.title.toLowerCase(), location: item.location.toLowerCase() },
                element
            );
            this.container.appendChild(element);
        });

        this.ResetButtonText();
    }

    /**
     * @brief Sorts burger locations by distance if geolocation is enabled, otherwise alphabetically.
     * @return {Array} Sorted burger data.
     */
    SortBurgerData(userLat, userLon, isGeolocationAllowed)
    {
        return isGeolocationAllowed
            ? this.burgerData
                .map(({ geodata, ...burger }) => ({
                    ...burger,
                    distance: this.Haversine(userLat, userLon, geodata.lat, geodata.long)
                }))
                .sort((a, b) => a.distance - b.distance)
            : this.burgerData.sort((a, b) => a.title.localeCompare(b.title));
    }

    /**
     * @brief Calculates the great-circle distance between two geographic coordinates.
     * @return {number} Distance in miles.
     */
    Haversine(lat1, lon1, lat2, lon2)
    {
        const R = 3958.8;
        const ToRad = angle => angle * (Math.PI / 180);
        const dLat = ToRad(lat2 - lat1);
        const dLon = ToRad(lon2 - lon1);
        const a = Math.sin(dLat / 2) ** 2 + Math.cos(ToRad(lat1)) * Math.cos(ToRad(lat2)) * Math.sin(dLon / 2) ** 2;
        return R * (2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a)));
    }


    //! UI Helpers    ---

    /**
     * @brief Displays an alert message in a modal.
     */
    ShowAlert(message)
    {
        const modal = document.getElementById("error-modal"),
            messageElement = document.getElementById("error-message"),
            closeModal = () => (modal.style.display = "none");

        const lines = message.split("|").map(entry =>
        {
            const codeMatch = entry.match(/\$(\d+)/);
            const code = codeMatch ? parseInt(codeMatch[1]) : 0;
            const label = entry.replace(/\.*\$\d+/, '').trim();
            return { label, code };
        });

        const lineHTML = lines.map(({ label, code }) => `
        <div class="receipt-line">
            <span>${label}</span>
            <span>$${(code / 100).toFixed(2)}</span>
        </div>
    `).join("");

        const total = lines.reduce((sum, { code }) => sum + code, 0);

        messageElement.innerHTML = `
        ${lineHTML}
        <div class="receipt-total total">
            <span>Total</span>
            <span class="dots"></span>
            <span>$${(total / 100).toFixed(2)}</span> 
        </div>
    `;

        modal.style.display = "block";

        document.getElementById("close-modal").addEventListener("click", closeModal);
        window.addEventListener("click", ({ target }) => target === modal && closeModal());
    }


    /**
     * @brief Resets the button text to its default state.
     */
    ResetButtonText()
    {
        this.btn.textContent = this.defaultBtnText;
    }


    //! Static Properties    ---

    static StatusLabels = {
        401: "Unauthorized Access 🍔🔒",
        403: "Forbidden Zone 🛑",
        404: "No Burgers Found 😭",
        500: "Grill Malfunction 🔥",
    };

    static GeolocationErrors = {
        1: "Please allow location access.",
        2: "Location information unavailable. Try again.",
        3: "Request timed out. Try again."
    };


}

const burgerLocator = new BurgerLocator("burgers.json?11");
